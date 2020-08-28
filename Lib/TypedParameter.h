#ifndef TYPEDPARAMETER_H
#define TYPEDPARAMETER_H
#include <memory>
#include <string>
#include <vector>
#include <sstream>


class CmdParameters;
class DefParameter;


/**
 * @brief Defines and processes command line parameters.
 */
struct TypedParameter {
  using string = std::string;

	struct Values {
  	bool   bool_value = false;
  	int    int_value = -1;
  	float  float_value = -1.0f;
  	string string_value;
	};

  class List : public std::vector<std::unique_ptr<TypedParameter>> {
    using Parent = std::vector<std::unique_ptr<TypedParameter>>;
   public:
    TypedParameter *operator[] (int index);
    TypedParameter *operator[] (const char *key);

    void prepare_usage(
      vector<string> &disp_defaults,
      vector<string> &disp_params,
      bool add_help = true);

    bool process_unnamed(const char *curarg);

		void reset_values();
  };


  DefParameter  &def_param;

  TypedParameter(DefParameter &var);

  int    get_int_value()    { return m_values.int_value; }
  bool   get_bool_value()   { return m_values.bool_value; }
  float  get_float_value()  { return m_values.float_value; }
  string get_string_value() { return m_values.string_value; }

  bool detected() const { return m_detected; }
	void reset_values();
  bool parse_param(const char *curarg);
  virtual string usage();

protected:
  std::vector<string> m_prefixes;
  bool   m_detected = false;

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
  virtual const char *value_indicator() const = 0;
  virtual void default_indicator(std::ostringstream &os) = 0;
	virtual bool takes_value() const { return true; }
};

#endif // TYPEDPARAMETER_H
