#ifndef TYPEDPARAMETER_H
#define TYPEDPARAMETER_H
#include <memory>
#include <string>
#include <vector>

using StrList = std::vector<std::string>;

class CmdParameters;
class DefParameter;


/**
 * @brief Defines and processes command line parameters.
 */
struct TypedParameter {
  using string = std::string;

  struct Values {
    bool   m_detected   = false;
    int    int_value    = -1;
    float  float_value  = -1.0f;
    string string_value;
  };

  class List : public std::vector<std::unique_ptr<TypedParameter>> {
    using Parent = std::vector<std::unique_ptr<TypedParameter>>;
   public:
    TypedParameter *operator[] (int index);
    TypedParameter const *operator[] (int index) const;
    TypedParameter *operator[] (const char *key);

    bool process_unnamed(const char *curarg);

    void reset_values();
  };


  /**
   * Used to create a view of sorted items for a types parameter list,
   * for help display.
   *
   * The referred list should not be destroyed during its lifetime.
   */
  class SortedList : public std::vector<TypedParameter const *> {
  public:
    SortedList(List const &parameters);

    StrList prefixes_str(bool add_help = true) const;
    StrList defaults_str(bool add_help = true) const;
  };


  DefParameter  &def_param;

  TypedParameter(DefParameter &var, char const *value_indicator);

  bool   detected()         const { return m_values.m_detected; }
  int    get_int_value()    const { return m_values.int_value; }
  bool   get_bool_value()   const { return detected(); }
  float  get_float_value()  const { return m_values.float_value; }
  string get_string_value() const { return m_values.string_value; }

  void reset_values();
  bool parse_param(const char *curarg);
  virtual string usage() const;
  std::string prefixes_str() const;
  std::string defaults_str() const;

  bool operator<(TypedParameter const &rhs) const;

protected:
  std::vector<string> m_prefixes;
  std::string m_value_indicator;

  Values m_defaults;
  Values m_values;

  int  get_int_value(const string &param);
  void error(const string &msg) const;

  bool parse_string_param(const string &in_value);

  float  get_float_value (const string &param);
  string get_string_value(const string &param) {
    return get_param(param.c_str());
  }

  string get_param(const char *curarg);

private:
  virtual bool parse_param_internal(const string &in_value) = 0;
  virtual std::string default_indicator() const { return ""; }
  virtual bool takes_value() const { return true; }
};

#endif // TYPEDPARAMETER_H
