#ifndef USERPREFERENCES_H
#define USERPREFERENCES_H

#include <Preferences.h>
#include <vector>
#include <Arduino.h>
#include <string.h>
#include <type_traits>

// ---------------------------------------------------------------------------
// PrefMeta / PrefDef: ההגדרה של העדפה - רשומה constexpr ב-flash (2a.4).
// מפתח, טיפוס, אינדקס, מופע Preferences, ברירת מחדל וטווח. ב-RAM נשארים רק הערך ומצביע לרשומה.
// ---------------------------------------------------------------------------
struct PrefMeta
{
    const char *key;
    PreferenceType type;
    uint8_t index;
    Preferences *prefs;
};

// הטווח: התחום שיש לו משמעות ושהקוד מטפל בו נכון (לא מגבלת התפריט). בשלב זה תיעוד בלבד - אף קוד לא קורא אותו.
// PR_MIN_MAX: min..max (ב-String: אורך בבתים); step - מידע לממשק בלבד.
// PR_LIST: רשימת ערכים. PR_OPEN: הטווח לא ידוע בוודאות מהקוד.
enum PrefRangeKind : uint8_t
{
    PR_OPEN,
    PR_MIN_MAX,
    PR_LIST
};

template <typename R>
struct PrefRange
{
    PrefRangeKind kind;
    R min;
    R max;
    R step;
    const R *list;
    uint8_t count;
};

// טיפוס ברירת המחדל בטבלה (ב-String: מחרוזת קבועה) וטיפוס הגבולות (ב-String: אורך)
template <typename T>
struct PrefStore
{
    using def_type = T;
    using range_type = T;
};
template <>
struct PrefStore<String>
{
    using def_type = const char *;
    using range_type = uint16_t;
};

template <typename T>
struct PrefDef : PrefMeta
{
    using D = typename PrefStore<T>::def_type;
    using R = typename PrefStore<T>::range_type;
    D def;
    PrefRange<R> range;

    // אותם ארגומנטים ובאותו סדר כמו הבנאי הקודם של UserPref, ובסוף הטווח
    constexpr PrefDef(const char *key, D d, PreferenceType type, uint8_t idx, Preferences *prefs, PrefRangeKind open)
        : PrefMeta{key, type, idx, prefs}, def(d), range{open, R(), R(), R(), nullptr, 0} {}
    constexpr PrefDef(const char *key, D d, PreferenceType type, uint8_t idx, Preferences *prefs,
                      PrefRangeKind kind, R min, R max, R step = 1)
        : PrefMeta{key, type, idx, prefs}, def(d), range{kind, min, max, step, nullptr, 0} {}
    template <size_t N>
    constexpr PrefDef(const char *key, D d, PreferenceType type, uint8_t idx, Preferences *prefs, const R (&list)[N])
        : PrefMeta{key, type, idx, prefs}, def(d), range{PR_LIST, R(), R(), R(), list, static_cast<uint8_t>(N)} {}
};

// בדיקת זמן קומפילציה (static_assert בקובץ השכבה): ברירת המחדל בתוך הטווח. לא לרשומות PR_OPEN.
template <typename T>
constexpr bool prefDefaultInRange(const PrefDef<T> &d)
{
    if constexpr (std::is_same<T, String>::value)
    {
        // String: הטווח הוא אורך בבתים
        if (d.range.kind == PR_MIN_MAX)
        {
            size_t len = 0;
            while (d.def[len])
                len++;
            return d.range.min <= len && len <= d.range.max;
        }
    }
    else
    {
        if (d.range.kind == PR_LIST)
        {
            for (uint8_t i = 0; i < d.range.count; i++)
            {
                if (d.range.list[i] == d.def)
                    return true;
            }
            return false;
        }
        if (d.range.kind == PR_MIN_MAX)
        {
            return d.range.min <= d.def && d.def <= d.range.max;
        }
    }
    return false; // PR_OPEN: אין טווח לבדוק
}

// ---------------------------------------------------------------------------
// UserPrefBase: מחלקת בסיס לגישה גנרית לכל העדפות
// ---------------------------------------------------------------------------
class UserPrefBase
{
protected:
    const PrefMeta *_meta;
    bool _isModified;

public:
    UserPrefBase(const PrefMeta &meta)
        : _meta(&meta), _isModified(false) {}

    virtual ~UserPrefBase() {}

    // Getters
    const char *getKey() const { return _meta->key; }
    PreferenceType getType() const { return _meta->type; }
    uint8_t getIndex() const { return _meta->index; }
    bool isModified() const { return _isModified; }
    bool hasChangedFromDefault() const { return _hasChangedFromDefault(); }

    // Pure virtual functions
    virtual void load() = 0;
    virtual void save() = 0;
    virtual void reset() = 0;
    virtual void print() = 0;
    virtual std::vector<uint8_t> toBytes() = 0;
    virtual bool fromBytes(const uint8_t *data, size_t length) = 0;

protected:
    virtual bool _hasChangedFromDefault() const = 0;
    void setModified(bool modified) { _isModified = modified; }
};

template <typename T>
class UserPref : public UserPrefBase
{
private:
    T _value;

    // ההגדרה בטבלה (flash). ב-String ברירת המחדל היא const char *
    const PrefDef<T> &def() const { return static_cast<const PrefDef<T> &>(*_meta); }

public:
    UserPref(const PrefDef<T> &definition)
        : UserPrefBase(definition), _value(definition.def) {}

    // Getters & Setters
    // גישה לערך
    T getValue() const { return _value; }
    // גישה לערך ברירת המחדל
    T getDefault() const { return def().def; }
    void setValue(T newValue)
    {
        if (_value != newValue)
        {
            Serial.print("Updating ");
            Serial.print(getKey());
            Serial.print(" from ");
            Serial.print(_value);
            Serial.print(" to ");
            Serial.println(newValue);
            _value = newValue;
            setModified(true);
            save();
        }
        else
        {
            Serial.print("No change for ");
            Serial.println(getKey());
        }
    }

    // נוסיף גם אופרטור = לנוחות
    UserPref &operator=(T newValue)
    {
        setValue(newValue);
        return *this;
    }
    // פונקציה מיוחדת עבור String
    const char *c_str() const
    {
        if constexpr (std::is_same<T, String>::value)
        {
            return _value.c_str();
        }
        return nullptr;
    }
    // נוסיף גם אופרטור המרה אוטומטית לטיפוס T
    operator T() const
    {
        return _value;
    }
    void print() override
    {
        Serial.print("idx: ");
        Serial.print(getIndex());
        Serial.print(" Key: ");
        Serial.print(getKey());
        Serial.print(", Value: ");
        Serial.print(_value);
        Serial.print(", Default: ");
        Serial.print(def().def);
        Serial.print(", Modified: ");
        Serial.println(_isModified ? "Yes" : "No");
    }
    virtual void load() override
    {
        Preferences *prefs = _meta->prefs;
        if (!prefs)
        {
            Serial.println("Preferences pointer is NULL in load().");
            return;
        }
        // Serial.print("Loading preference: ");
        // Serial.print(_key);
        // Serial.print(" with default value: ");
        // Serial.println(_defaultValue);
        if constexpr (std::is_same<T, uint8_t>::value)
        {
            _value = prefs->getUChar(getKey(), def().def);
        }
        else if constexpr (std::is_same<T, int8_t>::value)
        {
            _value = prefs->getChar(getKey(), def().def);
        }
        else if constexpr (std::is_same<T, uint16_t>::value)
        {
            _value = prefs->getUShort(getKey(), def().def);
        }
        else if constexpr (std::is_same<T, int16_t>::value)
        {
            _value = prefs->getShort(getKey(), def().def);
        }
        else if constexpr (std::is_same<T, String>::value)
        {
            _value = prefs->getString(getKey(), def().def);
        }
        else if constexpr (std::is_same<T, double>::value)
        {
            _value = prefs->getDouble(getKey(), def().def);
        }
        else
        {
            Serial.println("Unsupported type in load().");
        }
        // Serial.print("Loaded value for ");
        // Serial.print(_key);
        // Serial.print(": ");
        // Serial.println(_value);
    }

    virtual void save() override
    {
        Preferences *prefs = _meta->prefs;
        if (!prefs)
        {
            Serial.println("Preferences pointer is NULL in save().");
            return;
        }
        // Serial.print("Saving preference: ");
        // Serial.print(_key);
        // Serial.print(" with value: ");
        // Serial.println(_value);
        if constexpr (std::is_same<T, uint8_t>::value)
        {
            prefs->putUChar(getKey(), _value);
        }
        else if constexpr (std::is_same<T, int8_t>::value)
        {
            prefs->putChar(getKey(), _value);
        }
        else if constexpr (std::is_same<T, uint16_t>::value)
        {
            prefs->putUShort(getKey(), _value);
        }
        else if constexpr (std::is_same<T, int16_t>::value)
        {
            prefs->putShort(getKey(), _value);
        }
        else if constexpr (std::is_same<T, String>::value)
        {
            prefs->putString(getKey(), _value);
        }
        else if constexpr (std::is_same<T, double>::value)
        {
            prefs->putDouble(getKey(), _value);
        }
        else
        {
            Serial.println("Unsupported type in save().");
        }
    }

    virtual void reset() override
    {
        // Serial.print("Resetting preference: ");
        // Serial.println(_key);
        _value = def().def;
        _isModified = false;
        save();
    }

protected:
    bool _hasChangedFromDefault() const override
    {
        return _value != def().def;
    }

    std::vector<uint8_t> toBytes() override
    {
        std::vector<uint8_t> data;

        if constexpr (std::is_same<T, uint8_t>::value)
        {
            data.push_back(_value);
        }
        else if constexpr (std::is_same<T, int8_t>::value)
        {
            data.push_back(static_cast<uint8_t>(_value));
        }
        else if constexpr (std::is_same<T, uint16_t>::value)
        {
            uint16_t val = _value;
            data.push_back(val & 0xFF);        // Byte 1 - LSB
            data.push_back((val >> 8) & 0xFF); // Byte 2 - MSB
        }
        else if constexpr (std::is_same<T, int16_t>::value)
        {
            int16_t val = _value;
            data.push_back(val & 0xFF);
            data.push_back((val >> 8) & 0xFF);
        }
        else if constexpr (std::is_same<T, uint32_t>::value)
        {
            uint32_t val = _value;
            for (int i = 0; i < 4; i++)
            {
                data.push_back((val >> (8 * i)) & 0xFF);
            }
        }
        else if constexpr (std::is_same<T, int32_t>::value)
        {
            int32_t val = _value;
            for (int i = 0; i < 4; i++)
            {
                data.push_back((val >> (8 * i)) & 0xFF);
            }
        }
        else if constexpr (std::is_same<T, double>::value)
        {
            double val = _value;
            uint8_t *ptr = reinterpret_cast<uint8_t *>(&val);
            for (size_t i = 0; i < sizeof(double); i++)
            {
                data.push_back(ptr[i]);
            }
        }
        else if constexpr (std::is_same<T, String>::value)
        {
            for (size_t i = 0; i < _value.length(); i++)
            {
                data.push_back(static_cast<uint8_t>(_value[i]));
            }
            data.push_back(0); // סימון סוף מחרוזת
        }

        return data;
    }

    bool fromBytes(const uint8_t *data, size_t length) override
    {
        if (length < 1)
            return false;

        try
        {
            if constexpr (std::is_same<T, uint8_t>::value)
            {
                _value = data[0];
            }
            else if constexpr (std::is_same<T, int8_t>::value)
            {
                _value = static_cast<int8_t>(data[0]);
            }
            else if constexpr (std::is_same<T, uint16_t>::value)
            {
                if (length < 2) return false;
                _value = static_cast<uint16_t>(data[0] | (data[1] << 8));
            }
            else if constexpr (std::is_same<T, int16_t>::value)
            {
                if (length < 2) return false;
                _value = static_cast<int16_t>(data[0] | (data[1] << 8));
            }
            else if constexpr (std::is_same<T, uint32_t>::value)
            {
                if (length < 4) return false;
                uint32_t val = 0;
                for (int i = 0; i < 4; i++)
                {
                    val |= static_cast<uint32_t>(data[i]) << (8 * i);
                }
                _value = val;
            }
            else if constexpr (std::is_same<T, int32_t>::value)
            {
                if (length < 4) return false;
                int32_t val = 0;
                for (int i = 0; i < 4; i++)
                {
                    val |= static_cast<int32_t>(data[i]) << (8 * i);
                }
                _value = val;
            }
            else if constexpr (std::is_same<T, double>::value)
            {
                if (length < sizeof(double)) return false;
                double val;
                memcpy(&val, &data[0], sizeof(double));
                _value = val;
            }
            else if constexpr (std::is_same<T, String>::value)
            {
                const char *s = reinterpret_cast<const char *>(data);
                _value = String(s, strnlen(s, length));
            }

            return true;
        }
        catch (...)
        {
            Serial.println("Error in fromBytes conversion");
            return false;
        }
    }
};

// ---------------------------------------------------------------------------
// PrefList: רשימה קבועה של העדפות - מצביע להתחלה ומספר רשומות (2a.4, במקום std::vector)
// ---------------------------------------------------------------------------
struct PrefList
{
    UserPrefBase *const *first;
    size_t count;

    constexpr UserPrefBase *const *begin() const { return first; }
    constexpr UserPrefBase *const *end() const { return first + count; }
    constexpr size_t size() const { return count; }
    UserPrefBase *operator[](size_t i) const { return first[i]; }
};

// ---------------------------------------------------------------------------
// UserPreferences: מחלקה המאחדת את כל העדפות המשתמש
// ---------------------------------------------------------------------------
class UserPreferences
{
public:
    // הגדרת העדפות כמשתנים ציבוריים לגישה ישירה
    UserPref<uint8_t> forceWrite;

    // כל ההעדפות (UserPrefBase) לגישה גנרית: מערך קבוע אחד ב-flash (ההגדרה ב-UserPreferences.cpp).
    // הסדר: 50 הטקסטים ואחריהם ההעדפות הפשוטות (סדר הטעינה). שלוש הרשימות הן תצוגות עליו.
    static constexpr size_t numTextPrefs = 50;
    static constexpr size_t numSimplePrefs = 75;
    static UserPrefBase *const allPrefsArray[numTextPrefs + numSimplePrefs];
    static constexpr PrefList allPrefs{allPrefsArray, numTextPrefs + numSimplePrefs};
    static constexpr PrefList textPrefs{allPrefsArray, numTextPrefs};
    static constexpr PrefList simplePrefs{allPrefsArray + numTextPrefs, numSimplePrefs};

    // מופע Preferences שמשמש את כל העדפות המשתמש
    Preferences prefs;

    // בנאי: אתחול כל העדפות עם אותו מופע Preferences
    UserPreferences();

    ~UserPreferences()
    {
        prefs.end();
        Serial.println("UserPreferences closed.");
    }

    // Separate initialization method for Preferences:
    void init(const char *namespaceName)
    {
        prefs.begin(namespaceName, false);
        // Serial.print("UserPreferences initialized with namespace: ");
        // Serial.println(namespaceName);
    }

    // פונקציות עזר לגישה גנרית לכל ההעדפות
    void loadAll()
    {
        // Serial.println("Loading all user preferences...");
        for (auto pref : allPrefs)
        {
            pref->load();
        }
        // Serial.println("Finished loading preferences.");
    }

    // איפוס העדפות בלבד - לא נוגע בטקסטים
    void resetSimplePrefs()
    {
        for (auto pref : simplePrefs)
        {
            pref->reset();
        }
    }

    // איפוס טקסטים בלבד - לא נוגע בהעדפות
    void resetTextPrefs()
    {
        for (auto pref : textPrefs)
        {
            pref->reset();
        }
    }
    
    void printAll()
    {
        Serial.println("Printing all user preferences:");
        for (auto pref : allPrefs)
        {
            pref->print();
        }
    }
};
void setupPreferences();
#endif
