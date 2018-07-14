#ifndef CMDPARAMETER_H
#define CMDPARAMETER_H
#include <memory>
#include <string>
#include <vector>
#include <sstream>


class CmdDefinition;
class DefParameter;


/**
 * @brief Defines and processes command line parameters.
 */
struct CmdParameter {
  using string = std::string;

  class List : public std::vector<std::unique_ptr<CmdParameter>> {
    using Parent = std::vector<std::unique_ptr<CmdParameter>>;
   public:
    CmdParameter *operator[] (int index);
    CmdParameter *operator[] (const char *key);

    void prepare_usage(
      vector<string> &disp_defaults,
      vector<string> &disp_params);
    bool process_unnamed(const char *curarg);
  };


  DefParameter  &def_param;

  CmdParameter(DefParameter &var);

  int    get_int_value()    { return int_value; }
#ifndef LITE
  bool   get_bool_value()   { return bool_value; }
  float  get_float_value()  { return float_value; }
  string get_string_value() { return string_value; }

#endif  // LITE
  bool detected() const { return m_detected; }
  bool parse_param(const char *curarg);


protected:
	string m_prefix;
  bool   m_detected{false};
  int    int_value{-1};

  int  get_int_value(const string &param);
  bool parse_bool_param(const string &in_value);
	void error(const string &msg) const;
#ifndef LITE

  // Yes, crappy. Go ahead and enjoy your nausea.
  bool    bool_value{false};
  float   float_value{-1.0f};
  string  string_value;

  bool parse_string_param(const string &in_value);

  float  get_float_value (const string &param);
  string get_string_value(const string &param) {
    return get_param(param.c_str());
  }

#endif  // LITE
  string get_param(const char *curarg);

private:
  bool set_default();

  virtual bool parse_param_internal(const string &in_value) = 0;
  virtual const char *value_indicator() const = 0;
  virtual void default_indicator(std::ostringstream &os) = 0;
	virtual bool takes_value() const { return true; }
};

#endif // CMDPARAMETER_H
