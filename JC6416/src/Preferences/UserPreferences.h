#ifndef USERPREFERENCES_H
#define USERPREFERENCES_H

#include <Preferences.h>
#include <vector>
#include <Arduino.h>
#include <string.h>
#include <type_traits>

// ---------------------------------------------------------------------------
// UserPrefBase: מחלקת בסיס לגישה גנרית לכל העדפות
// ---------------------------------------------------------------------------
class UserPrefBase
{
protected:
    const char *_key;
    PreferenceType _type;
    uint8_t _index;
    bool _isModified;
    Preferences *_prefs;

public:
    UserPrefBase(const char *key, PreferenceType type, uint8_t idx, Preferences *prefs)
        : _key(key), _type(type), _index(idx), _isModified(false), _prefs(prefs) {}

    virtual ~UserPrefBase() {}

    // Getters
    const char *getKey() const { return _key; }
    PreferenceType getType() const { return _type; }
    uint8_t getIndex() const { return _index; }
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
    T _defaultValue;

public:
    UserPref(const char *key, T defaultValue, PreferenceType type, uint8_t idx, Preferences *prefs)
        : UserPrefBase(key, type, idx, prefs), _value(defaultValue), _defaultValue(defaultValue) {}

    // Getters & Setters
    // גישה לערך
    T getValue() const { return _value; }
    // גישה לערך ברירת המחדל
    T getDefault() const { return _defaultValue; }
    void setValue(T newValue)
    {
        if (_value != newValue)
        {
            Serial.print("Updating ");
            Serial.print(_key);
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
            Serial.println(_key);
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
        Serial.print(_index);
        Serial.print(" Key: ");
        Serial.print(_key);
        Serial.print(", Value: ");
        Serial.print(_value);
        Serial.print(", Default: ");
        Serial.print(_defaultValue);
        Serial.print(", Modified: ");
        Serial.println(_isModified ? "Yes" : "No");
    }
    virtual void load() override
    {
        if (!_prefs)
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
            _value = _prefs->getUChar(_key, _defaultValue);
        }
        else if constexpr (std::is_same<T, int8_t>::value)
        {
            _value = _prefs->getChar(_key, _defaultValue);
        }
        else if constexpr (std::is_same<T, uint16_t>::value)
        {
            _value = _prefs->getUShort(_key, _defaultValue);
        }
        else if constexpr (std::is_same<T, int16_t>::value)
        {
            _value = _prefs->getShort(_key, _defaultValue);
        }
        else if constexpr (std::is_same<T, String>::value)
        {
            _value = _prefs->getString(_key, _defaultValue);
        }
        else if constexpr (std::is_same<T, double>::value)
        {
            _value = _prefs->getDouble(_key, _defaultValue);
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
        if (!_prefs)
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
            _prefs->putUChar(_key, _value);
        }
        else if constexpr (std::is_same<T, int8_t>::value)
        {
            _prefs->putChar(_key, _value);
        }
        else if constexpr (std::is_same<T, uint16_t>::value)
        {
            _prefs->putUShort(_key, _value);
        }
        else if constexpr (std::is_same<T, int16_t>::value)
        {
            _prefs->putShort(_key, _value);
        }
        else if constexpr (std::is_same<T, String>::value)
        {
            _prefs->putString(_key, _value);
        }
        else if constexpr (std::is_same<T, double>::value)
        {
            _prefs->putDouble(_key, _value);
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
        _value = _defaultValue;
        _isModified = false;
        save();
    }

protected:
    bool _hasChangedFromDefault() const override
    {
        return _value != _defaultValue;
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
// UserPreferences: מחלקה המאחדת את כל העדפות המשתמש
// ---------------------------------------------------------------------------
class UserPreferences
{
public:
    // הגדרת העדפות כמשתנים ציבוריים לגישה ישירה
    UserPref<uint8_t> forceWrite;

    // וקטור שמאגד את כל ההעדפות (UserPrefBase) לגישה גנרית
    std::vector<UserPrefBase *> allPrefs;
    std::vector<UserPrefBase *> textPrefs;
    std::vector<UserPrefBase *> simplePrefs;

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
