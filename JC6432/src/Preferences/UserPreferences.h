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
                _value = String(reinterpret_cast<const char *>(&data[0]));
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
    UserPref<String> textInput1;
    UserPref<String> textInput2;
    UserPref<String> textInput3;
    UserPref<String> textInput4;
    UserPref<String> textInput5;
    UserPref<String> textInput6;
    UserPref<String> textInput7;
    UserPref<String> textInput8;
    UserPref<String> textInput9;
    UserPref<String> textInput10;
    UserPref<String> textInput11;
    UserPref<String> textInput12;
    UserPref<String> textInput13;
    UserPref<String> textInput14;
    UserPref<String> textInput15;
    UserPref<String> textInput16;
    UserPref<String> textInput17;
    UserPref<String> textInput18;
    UserPref<String> textInput19;
    UserPref<String> textInput20;
    UserPref<String> textInput21;
    UserPref<String> textInput22;
    UserPref<String> textInput23;
    UserPref<String> textInput24;
    UserPref<String> textInput25;
    UserPref<String> textInput26;
    UserPref<String> textInput27;
    UserPref<String> textInput28;
    UserPref<String> textInput29;
    UserPref<String> textInput30;
    UserPref<String> textInput31;
    UserPref<String> textInput32;
    UserPref<String> textInput33;
    UserPref<String> textInput34;
    UserPref<String> textInput35;
    UserPref<String> textInput36;
    UserPref<String> textInput37;
    UserPref<String> textInput38;
    UserPref<String> textInput39;
    UserPref<String> textInput40;
    UserPref<String> textInput41;
    UserPref<String> textInput42;
    UserPref<String> textInput43;
    UserPref<String> textInput44;
    UserPref<String> textInput45;
    UserPref<String> textInput46;
    UserPref<String> textInput47;
    UserPref<String> textInput48;
    UserPref<String> textInput49;
    UserPref<String> textInput50;
    UserPref<String> textPlace;
    UserPref<uint8_t> dayIndex;
    UserPref<uint8_t> shita;
    UserPref<uint8_t> eda;
    UserPref<uint8_t> ofek;
    UserPref<double_t> nz_lat;
    UserPref<double_t> nz_long;
    UserPref<int16_t> nz_height;
    UserPref<uint8_t> nz_hul;
    UserPref<int8_t> timezone;
    UserPref<uint8_t> brightness;
    UserPref<uint8_t> safa;
    UserPref<uint8_t> colorUp;
    UserPref<uint8_t> colorMiddle;
    UserPref<uint8_t> colorBottomT;
    UserPref<uint8_t> colorBottomV;
    UserPref<uint8_t> colorYom;
    UserPref<uint8_t> colorParasha;
    UserPref<uint8_t> colorIluy;
    UserPref<uint8_t> clockFont;
    UserPref<uint8_t> dateFormat;
    UserPref<uint8_t> clockFormat;
    UserPref<uint8_t> timeFormat;
    UserPref<uint8_t> shaonKaiz;
    UserPref<uint8_t> middleLine;
    UserPref<uint8_t> upLine;
    UserPref<uint8_t> zriha;
    UserPref<uint8_t> shkiaa;
    UserPref<uint8_t> rabenuTam;
    UserPref<uint8_t> zeetCochavim;
    UserPref<uint8_t> zmanTalit;
    UserPref<uint8_t> alotHashahar;
    UserPref<uint8_t> minhaG;
    UserPref<uint8_t> sozksMga;
    UserPref<uint8_t> soztMga;
    UserPref<uint8_t> plagMinha;
    UserPref<uint8_t> zeetShabat;
    UserPref<uint8_t> zeetZom;
    UserPref<uint8_t> nerot;
    UserPref<uint8_t> homa;
    UserPref<uint8_t> hul;
    UserPref<uint8_t> leil;
    UserPref<uint8_t> netzClk;
    UserPref<uint8_t> shanim;
    UserPref<uint16_t> interval;
    UserPref<uint16_t> intervalIluy;
    UserPref<uint16_t> intervalTime;
    UserPref<uint16_t> intervalInfo;
    UserPref<uint8_t> dafHayomi;
    UserPref<uint8_t> yerushalmi;
    UserPref<uint8_t> yom;
    UserPref<uint8_t> parasha;
    UserPref<uint8_t> haftara;
    UserPref<uint8_t> ivri;
    UserPref<uint8_t> loazi;
    UserPref<uint8_t> tfila;
    UserPref<uint8_t> moadim;
    UserPref<uint8_t> showIsraeli;
    UserPref<uint8_t> TAZ;
    UserPref<uint8_t> SOZT;
    UserPref<uint8_t> SOZKS;
    UserPref<uint8_t> HMM;
    UserPref<uint8_t> PSZ;
    UserPref<uint8_t> HAZ;
    UserPref<uint8_t> ZSZ1;
    UserPref<uint8_t> ZSZ;
    UserPref<uint8_t> ZYS;
    UserPref<uint8_t> ZPS;
    UserPref<uint8_t> IPO;
    UserPref<uint8_t> IORT;
    UserPref<uint8_t> colorIvri;
    UserPref<uint8_t> colorLoazi;
    UserPref<uint8_t> colorParashaV;
    UserPref<uint8_t> screen;
    UserPref<uint8_t> bt;

    // וקטור שמאגד את כל ההעדפות (UserPrefBase) לגישה גנרית
    std::vector<UserPrefBase *> allPrefs;
    std::vector<UserPrefBase *> textPrefs;
    std::vector<UserPrefBase *> simplePrefs;

    // מופע Preferences שמשמש את כל העדפות המשתמש
    Preferences prefs;

    // בנאי: אתחול כל העדפות עם אותו מופע Preferences
    UserPreferences() : forceWrite("forceWrite", 0, PT_U8, 1, &prefs),
                        textInput1("textInput1", "", PT_STR, 0, &prefs),
                        textInput2("textInput2", "", PT_STR, 0, &prefs),
                        textInput3("textInput3", "", PT_STR, 0, &prefs),
                        textInput4("textInput4", "", PT_STR, 0, &prefs),
                        textInput5("textInput5", "", PT_STR, 0, &prefs),
                        textInput6("textInput6", "", PT_STR, 0, &prefs),
                        textInput7("textInput7", "", PT_STR, 0, &prefs),
                        textInput8("textInput8", "", PT_STR, 0, &prefs),
                        textInput9("textInput9", "", PT_STR, 0, &prefs),
                        textInput10("textInput10", "", PT_STR, 0, &prefs),
                        textInput11("textInput11", "", PT_STR, 0, &prefs),
                        textInput12("textInput12", "", PT_STR, 0, &prefs),
                        textInput13("textInput13", "", PT_STR, 0, &prefs),
                        textInput14("textInput14", "", PT_STR, 0, &prefs),
                        textInput15("textInput15", "", PT_STR, 0, &prefs),
                        textInput16("textInput16", "", PT_STR, 0, &prefs),
                        textInput17("textInput17", "", PT_STR, 0, &prefs),
                        textInput18("textInput18", "", PT_STR, 0, &prefs),
                        textInput19("textInput19", "", PT_STR, 0, &prefs),
                        textInput20("textInput20", "", PT_STR, 0, &prefs),
                        textInput21("textInput21", "", PT_STR, 0, &prefs),
                        textInput22("textInput22", "", PT_STR, 0, &prefs),
                        textInput23("textInput23", "", PT_STR, 0, &prefs),
                        textInput24("textInput24", "", PT_STR, 0, &prefs),
                        textInput25("textInput25", "", PT_STR, 0, &prefs),
                        textInput26("textInput26", "", PT_STR, 0, &prefs),
                        textInput27("textInput27", "", PT_STR, 0, &prefs),
                        textInput28("textInput28", "", PT_STR, 0, &prefs),
                        textInput29("textInput29", "", PT_STR, 0, &prefs),
                        textInput30("textInput30", "", PT_STR, 0, &prefs),
                        textInput31("textInput31", "", PT_STR, 0, &prefs),
                        textInput32("textInput32", "", PT_STR, 0, &prefs),
                        textInput33("textInput33", "", PT_STR, 0, &prefs),
                        textInput34("textInput34", "", PT_STR, 0, &prefs),
                        textInput35("textInput35", "", PT_STR, 0, &prefs),
                        textInput36("textInput36", "", PT_STR, 0, &prefs),
                        textInput37("textInput37", "", PT_STR, 0, &prefs),
                        textInput38("textInput38", "", PT_STR, 0, &prefs),
                        textInput39("textInput39", "", PT_STR, 0, &prefs),
                        textInput40("textInput40", "", PT_STR, 0, &prefs),
                        textInput41("textInput41", "", PT_STR, 0, &prefs),
                        textInput42("textInput42", "", PT_STR, 0, &prefs),
                        textInput43("textInput43", "", PT_STR, 0, &prefs),
                        textInput44("textInput44", "", PT_STR, 0, &prefs),
                        textInput45("textInput45", "", PT_STR, 0, &prefs),
                        textInput46("textInput46", "", PT_STR, 0, &prefs),
                        textInput47("textInput47", "", PT_STR, 0, &prefs),
                        textInput48("textInput48", "", PT_STR, 0, &prefs),
                        textInput49("textInput49", "", PT_STR, 0, &prefs),
                        textInput50("textInput50", "", PT_STR, 0, &prefs),
                        
                        textPlace("textPlace", "whaht otwun", PT_STR, 2, &prefs),
                        // input custom place name
                        
                        dayIndex("dayIndex", 1, PT_U8, 3, &prefs),
                        // TODO
                        
                        shita("shita", 0, PT_U8, 4, &prefs),
                        // 0. Or hachayim
                        // 1. Kise rachamim
                        // 2. chazon shamaim
                        // 3. harav melamed
                        // 4. habad
                        // 5. habad - harav singawi
                        // 6. harav tucazinsky
                        // 7. ben ish chai
                        
                        eda("eda", 1, PT_U8, 5, &prefs),
                        // 1. Edot hamizrah
                        // 2. sfarad
                        // 3. ashkenaz
                        // 4. teiman - baladi
                        // 5. teiman - shami
                        // 6. chabad
                        
                        ofek("ofek", 1, PT_U8, 6, &prefs),
                        // ofek index
                        
                        nz_lat("nz_lat", 32.07, PT_BLOB, 7, &prefs),
                        // latitude (valid values -90 to 90)
                        
                        nz_long("nz_long", 34.78, PT_BLOB, 8, &prefs),
                        // longitude (valid values -180 to 180)
                        
                        nz_height("nz_height", 20, PT_I16, 9, &prefs),
                        // city height
                        
                        nz_hul("nz_hul", 0, PT_U8, 10, &prefs),
                        // place country
                        // 0. israel
                        // 1. hul
                        
                        timezone("timezone", 2, PT_I8, 11, &prefs),
                        // timezone (valid values -12 to 14)
                        
                        brightness("brightness", 200, PT_U8, 12, &prefs),
                        // 1-12  multiple by 20. (valid values 0-255)
                        
                        safa("safa", 1, PT_U8, 13, &prefs),
                        // 1. anglit
                        // 2. rusit
                        
                        colorUp("colorUp", 1, PT_U8, 14, &prefs),
                        // 1-7
                        
                        colorMiddle("colorMiddle", 6, PT_U8, 15, &prefs),
                        // 1-7
                        
                        colorBottomT("colorBottomT", 1, PT_U8, 16, &prefs),
                        // 1-7
                        
                        colorBottomV("colorBottomV", 6, PT_U8, 17, &prefs),
                        // 1-7
                        
                        colorYom("colorYom", 1, PT_U8, 18, &prefs),
                        // 1-7
                        
                        colorParasha("colorParasha", 1, PT_U8, 19, &prefs),
                        // 1-7
                        
                        colorIluy("colorIluy", 6, PT_U8, 20, &prefs),
                        // 1-7
                        
                        clockFont("clockFont", 0, PT_U8, 21, &prefs),
                        // 0-4
                        
                        dateFormat("dateFormat", 1, PT_U8, 22, &prefs),
                        // 1. 14/03/2023
                        // 2. 14/3/2023
                        // 3. 14/3/23
                        // 4. 14/3
                        // 5. 03/14/2023
                        // 6. 2023-03-14
                        // 7. 14.03.2023
                        // 8. without loazi date
                        
                        clockFormat("clockFormat", 24, PT_U8, 23, &prefs),
                        // 12
                        // 24
                        
                        timeFormat("timeFormat", 0, PT_U8, 24, &prefs),
                        // bool. leading zero in time
                        
                        shaonKaiz("shaonKaiz", 1, PT_U8, 25, &prefs),
                        // 1. automatic
                        // 2. shaon kaiz, timeZone +1
                        // 3. shaon horef, timeZone
                        
                        middleLine("middleLine", 1, PT_U8, 26, &prefs),
                        // 1. Clock only
                        // 2. Clock + Day
                        
                        upLine("upLine", 1, PT_U8, 27, &prefs),
                        // 1. Heb date + Loazi Date
                        // 2. Heb date + Day
                        // 3. Heb date only
                        
                        zriha("zriha", 2, PT_U8, 28, &prefs),
                        // 1. Mishorit
                        // 2. Haniret
                        // 3. Astronomit
                        
                        shkiaa("shkiaa", 3, PT_U8, 29, &prefs),
                        // 1. Mishorit
                        // 2. Haniret
                        // 3. Astronomit
                        
                        rabenuTam("rabenuTam", 20, PT_U8, 30, &prefs),
                        // hishuv rabenu tam
                        
                        zeetCochavim("zeetCochavim", 8, PT_U8, 31, &prefs),
                        // hishuv zeet cochavim
                        
                        zmanTalit("zmanTalit", 4, PT_U8, 32, &prefs),
                        // hishuv zman talit utfilin
                        
                        alotHashahar("alotHashahar", 4, PT_U8, 33, &prefs),
                        // hishuv zman alot hashahar
                        
                        minhaG("minhaG", 3, PT_U8, 34, &prefs),
                        // hishuv zman minha gdola
                        
                        sozksMga("sozksMga", 1, PT_U8, 35, &prefs),
                        // hishuv zman sof zman kriat shma mga
                        
                        soztMga("soztMga", 1, PT_U8, 36, &prefs),
                        // hishuv zman sof zman tfila mga
                        
                        plagMinha("plagMinha", 1, PT_U8, 37, &prefs),
                        // hishuv zman plag haminha
                        
                        zeetShabat("zeetShabat", 16, PT_U8, 38, &prefs),
                        // hishuv zman zeet shabat
                        
                        zeetZom("zeetZom", 14, PT_U8, 39, &prefs),
                        // hishuv zman zeet zom derabanan
                        
                        nerot("nerot", 22, PT_U8, 40, &prefs),
                        // hadlakat nerot mukdemet
                        // 0 - 40
                        
                        homa("homa", 0, PT_U8, 41, &prefs),
                        // 0. lo mukefet homa
                        // 1. mukefet homa
                        
                        hul("hul", 0, PT_U8, 42, &prefs),
                        // 0. israel
                        // 1. bnei hul
                        
                        leil("leil", 1, PT_U8, 43, &prefs),
                        // bool. after shkiaa
                        // 0. yom today
                        // 1. leil tomorrow
                        
                        netzClk("netzClk", 0, PT_U8, 44, &prefs),
                        // time left to Netz
                        // 0-60
                        
                        shanim("shanim", 0, PT_U8, 45, &prefs),
                        // time from hurban bayit sheni
                        // 0. 1954
                        // 1. 1955
                        
                        interval("interval", 30, PT_U16, 46, &prefs),
                        // change rate in seconds
                        // 5-600 (multiple by 5)
                        // use in 4020 only
                        
                        intervalIluy("intervalIluy", 30, PT_U16, 47, &prefs),
                        // change rate in seconds
                        // 5-600 (multiple by 5)
                        // use in 6416 only
                        
                        intervalTime("intervalTime", 30, PT_U16, 48, &prefs),
                        // change rate in seconds
                        // 5-600 (multiple by 5)
                        // use in 6416 only
                        
                        intervalInfo("intervalInfo", 30, PT_U16, 49, &prefs),
                        // change rate in seconds
                        // 5-600 (multiple by 5)
                        // use in 6416 only
                        
                        dafHayomi("dafHayomi", 1, PT_U8, 50, &prefs),
                        // bool. show Daf hayomi bavli
                        
                        yerushalmi("yerushalmi", 0, PT_U8, 51, &prefs),
                        // bool. show Daf hayomi yerushalmi
                        
                        yom("yom", 0, PT_U8, 52, &prefs),
                        // bool. show yom bashavua
                        
                        parasha("parasha", 0, PT_U8, 53, &prefs),
                        // bool. show parashat hashavua
                        
                        haftara("haftara", 1, PT_U8, 54, &prefs),
                        // bool. show haftarat hashavua
                        
                        ivri("ivri", 0, PT_U8, 55, &prefs),
                        // bool. show taarich ivri begadol
                        
                        loazi("loazi", 0, PT_U8, 56, &prefs),
                        // bool. show taarich loazi begadol
                        
                        tfila("tfila", 1, PT_U8, 57, &prefs),
                        // bool. show meida lamitpalel
                        
                        moadim("moadim", 1, PT_U8, 58, &prefs),
                        // bool. izcur hagim - moadim - zom - hodesh
                        
                        showIsraeli("showIsraeli", 1, PT_U8, 59, &prefs),
                        // bool. izcur moadim israelim
                        
                        TAZ("TAZ", 1, PT_U8, 60, &prefs),
                        // zman talit - alot hashachar - zriha
                        // 0. off
                        // 1. on time
                        // 2. all day
                        
                        SOZT("SOZT", 1, PT_U8, 61, &prefs),
                        // sof zman tfila
                        // 0. off
                        // 1. on time
                        // 2. all day
                        
                        SOZKS("SOZKS", 1, PT_U8, 62, &prefs),
                        // sof zman kriat shma
                        // 0. off
                        // 1. on time
                        // 2. all day
                        
                        HMM("HMM", 1, PT_U8, 63, &prefs),
                        // chazot - mincha gdola - mincha ktana
                        // 0. off
                        // 1. on time
                        // 2. all day
                        
                        PSZ("PSZ", 1, PT_U8, 64, &prefs),
                        // plag hamincha - shkiaa - zeet hacochavim
                        // 0. off
                        // 1. on time
                        // 2. all day
                        
                        HAZ("HAZ", 1, PT_U8, 65, &prefs),
                        // hazot - alot hashachar - zriha
                        // 0. off
                        // 1. on time
                        // 2. all day
                        
                        ZSZ1("ZSZ1", 1, PT_U8, 66, &prefs),
                        // zriha - shkiaa - zeet hacochavim
                        // shishi shabat
                        // 0. off
                        // 1. on time
                        // 2. all day
                        
                        ZSZ("ZSZ", 1, PT_U8, 67, &prefs),
                        // zriha - shkiaa - zeet hacochavim
                        // yom hol
                        // 0. off
                        // 1. on time
                        // 2. all day
                        
                        ZYS("ZYS", 3, PT_U8, 68, &prefs),
                        // zriha - yom - shkiaa
                        // 0. off
                        // 1. yom hol
                        // 2. shishi shabat
                        // 3. all week
                        
                        ZPS("ZPS", 0, PT_U8, 69, &prefs),
                        // zriha - parasha - shkiaa
                        // 0. off
                        // 1. yom hol
                        // 2. shishi shabat
                        // 3. all week
                        
                        IPO("IPO", 2, PT_U8, 70, &prefs),
                        // knisat shabat - parasha - zeet shabat
                        // 0. off
                        // 1. yom hol
                        // 2. shishi shabat
                        // 3. all week
                        
                        IORT("IORT", 2, PT_U8, 71, &prefs),
                        // knisat shabat - zeet shabat - rabenu tam
                        // 0. off
                        // 1. yom hol
                        // 2. shishi shabat
                        // 3. all week
                                                
                        colorIvri("colorIvri", 6, PT_U8, 72, &prefs),
                        // 1-7
                        
                        colorLoazi("colorLoazi", 1, PT_U8, 73, &prefs),
                        // 1-7
                        
                        colorParashaV("colorParashaV", 6, PT_U8, 74, &prefs),
                        // 1-7

                        screen("screen", 2, PT_U8, 75, &prefs),
                        // main screens
                        // 1. Big
                        // 2. Small

                        bt("bt", 1, PT_U8, 76, &prefs)
                        // bluetooth on/off
                        // 0. off
                        // 1. on
    {
        // הוספת ההעדפות לווקטור לגישה גנרית
        simplePrefs.push_back(&forceWrite);
        simplePrefs.push_back(&textPlace);
        simplePrefs.push_back(&dayIndex);
        simplePrefs.push_back(&shita);
        simplePrefs.push_back(&eda);
        simplePrefs.push_back(&ofek);
        simplePrefs.push_back(&nz_lat);
        simplePrefs.push_back(&nz_long);
        simplePrefs.push_back(&nz_height);
        simplePrefs.push_back(&nz_hul);
        simplePrefs.push_back(&timezone);
        simplePrefs.push_back(&brightness);
        simplePrefs.push_back(&safa);
        simplePrefs.push_back(&colorUp);
        simplePrefs.push_back(&colorMiddle);
        simplePrefs.push_back(&colorBottomT);
        simplePrefs.push_back(&colorBottomV);
        simplePrefs.push_back(&colorYom);
        simplePrefs.push_back(&colorParasha);
        simplePrefs.push_back(&colorIluy);
        simplePrefs.push_back(&clockFont);
        simplePrefs.push_back(&dateFormat);
        simplePrefs.push_back(&clockFormat);
        simplePrefs.push_back(&timeFormat);
        simplePrefs.push_back(&shaonKaiz);
        simplePrefs.push_back(&middleLine);
        simplePrefs.push_back(&upLine);
        simplePrefs.push_back(&zriha);
        simplePrefs.push_back(&shkiaa);
        simplePrefs.push_back(&rabenuTam);
        simplePrefs.push_back(&zeetCochavim);
        simplePrefs.push_back(&zmanTalit);
        simplePrefs.push_back(&alotHashahar);
        simplePrefs.push_back(&minhaG);
        simplePrefs.push_back(&sozksMga);
        simplePrefs.push_back(&soztMga);
        simplePrefs.push_back(&plagMinha);
        simplePrefs.push_back(&zeetShabat);
        simplePrefs.push_back(&zeetZom);
        simplePrefs.push_back(&nerot);
        simplePrefs.push_back(&homa);
        simplePrefs.push_back(&hul);
        simplePrefs.push_back(&leil);
        simplePrefs.push_back(&netzClk);
        simplePrefs.push_back(&shanim);
        simplePrefs.push_back(&interval);
        simplePrefs.push_back(&intervalIluy);
        simplePrefs.push_back(&intervalTime);
        simplePrefs.push_back(&intervalInfo);
        simplePrefs.push_back(&dafHayomi);
        simplePrefs.push_back(&yerushalmi);
        simplePrefs.push_back(&yom);
        simplePrefs.push_back(&parasha);
        simplePrefs.push_back(&haftara);
        simplePrefs.push_back(&ivri);
        simplePrefs.push_back(&loazi);
        simplePrefs.push_back(&tfila);
        simplePrefs.push_back(&moadim);
        simplePrefs.push_back(&showIsraeli);
        simplePrefs.push_back(&TAZ);
        simplePrefs.push_back(&SOZT);
        simplePrefs.push_back(&SOZKS);
        simplePrefs.push_back(&HMM);
        simplePrefs.push_back(&PSZ);
        simplePrefs.push_back(&HAZ);
        simplePrefs.push_back(&ZSZ1);
        simplePrefs.push_back(&ZSZ);
        simplePrefs.push_back(&ZYS);
        simplePrefs.push_back(&ZPS);
        simplePrefs.push_back(&IPO);
        simplePrefs.push_back(&IORT);
        simplePrefs.push_back(&colorIvri);
        simplePrefs.push_back(&colorLoazi);
        simplePrefs.push_back(&colorParashaV);
        simplePrefs.push_back(&screen);
        simplePrefs.push_back(&bt);

        textPrefs.push_back(&textInput1);
        textPrefs.push_back(&textInput2);
        textPrefs.push_back(&textInput3);
        textPrefs.push_back(&textInput4);
        textPrefs.push_back(&textInput5);
        textPrefs.push_back(&textInput6);
        textPrefs.push_back(&textInput7);
        textPrefs.push_back(&textInput8);
        textPrefs.push_back(&textInput9);
        textPrefs.push_back(&textInput10);
        textPrefs.push_back(&textInput11);
        textPrefs.push_back(&textInput12);
        textPrefs.push_back(&textInput13);
        textPrefs.push_back(&textInput14);
        textPrefs.push_back(&textInput15);
        textPrefs.push_back(&textInput16);
        textPrefs.push_back(&textInput17);
        textPrefs.push_back(&textInput18);
        textPrefs.push_back(&textInput19);
        textPrefs.push_back(&textInput20);
        textPrefs.push_back(&textInput21);
        textPrefs.push_back(&textInput22);
        textPrefs.push_back(&textInput23);
        textPrefs.push_back(&textInput24);
        textPrefs.push_back(&textInput25);
        textPrefs.push_back(&textInput26);
        textPrefs.push_back(&textInput27);
        textPrefs.push_back(&textInput28);
        textPrefs.push_back(&textInput29);
        textPrefs.push_back(&textInput30);
        textPrefs.push_back(&textInput31);
        textPrefs.push_back(&textInput32);
        textPrefs.push_back(&textInput33);
        textPrefs.push_back(&textInput34);
        textPrefs.push_back(&textInput35);
        textPrefs.push_back(&textInput36);
        textPrefs.push_back(&textInput37);
        textPrefs.push_back(&textInput38);
        textPrefs.push_back(&textInput39);
        textPrefs.push_back(&textInput40);
        textPrefs.push_back(&textInput41);
        textPrefs.push_back(&textInput42);
        textPrefs.push_back(&textInput43);
        textPrefs.push_back(&textInput44);
        textPrefs.push_back(&textInput45);
        textPrefs.push_back(&textInput46);
        textPrefs.push_back(&textInput47);
        textPrefs.push_back(&textInput48);
        textPrefs.push_back(&textInput49);
        textPrefs.push_back(&textInput50);

        allPrefs.insert(allPrefs.end(), textPrefs.begin(), textPrefs.end());
        allPrefs.insert(allPrefs.end(), simplePrefs.begin(), simplePrefs.end());
    }

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

    void saveAll()
    {
        Serial.println("Saving all user preferences...");
        for (auto pref : allPrefs)
        {
            pref->save();
        }
        Serial.println("Finished saving preferences.");
    }

    void resetAll()
    {
        Serial.println("Resetting all preferences to default...");
        for (auto pref : allPrefs)
        {
            pref->reset();
        }
        Serial.println("Finished resetting preferences.");
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

    // Template function to update a value by key in a generic way
    template <typename T>
    bool setValueByKey(const char *key, T newValue)
    {
        for (auto pref : allPrefs)
        {
            if (strcmp(pref->getKey(), key) == 0)
            {
                UserPref<T> *p = dynamic_cast<UserPref<T> *>(pref);
                if (p != nullptr)
                {
                    p->set(newValue);
                    return true;
                }
                else
                {
                    Serial.print("Type mismatch for key: ");
                    Serial.println(key);
                    return false;
                }
            }
        }
        Serial.print("Key not found: ");
        Serial.println(key);
        return false;
    }

    // Template function to get a value by key in a generic way
    template <typename T>
    T getValueByKey(const char *key, T defaultValue)
    {
        for (auto pref : allPrefs)
        {
            if (strcmp(pref->getKey(), key) == 0)
            {
                UserPref<T> *p = dynamic_cast<UserPref<T> *>(pref);
                if (p != nullptr)
                {
                    return p->getValue();
                }
                else
                {
                    Serial.print("Type mismatch for key: ");
                    Serial.println(key);
                    return defaultValue;
                }
            }
        }
        Serial.print("Key not found, returning default: ");
        Serial.println(defaultValue);
        return defaultValue;
    }
    // פונקציה חדשה לקבלת העדפה עם טיפוס ספציפי
    template <typename T>
    UserPref<T> *getPref(const char *key)
    {
        for (auto pref : allPrefs)
        {
            if (strcmp(pref->getKey(), key) == 0)
            {
                return dynamic_cast<UserPref<T> *>(pref);
            }
        }
        return nullptr;
    }
};
void setupPreferences();
#endif
