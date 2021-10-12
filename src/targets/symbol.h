#ifndef __OG_TARGETS_SYMBOL_H__
#define __OG_TARGETS_SYMBOL_H__

#include <vector>
#include <string>
#include <memory>
#include <cdk/types/basic_type.h>

namespace og {

  class symbol {

    std::shared_ptr<cdk::basic_type> _type;
    
    std::string _name;

    std::vector<std::shared_ptr<cdk::basic_type>> _args_types;

    long _value; // hack!

    int _qualifier;

    bool _initialized;

    int _offset = 0;

    bool _isFunction;

    bool _forward = false;

  public:
    symbol(std::shared_ptr<cdk::basic_type> type, const std::string &name, long value, int qualifier, bool initialized, bool isFunction, bool forward = false) :
        _type(type), _name(name), _value(value), _qualifier(qualifier), _initialized(initialized), _isFunction(isFunction), _forward(forward) {
    }

    virtual ~symbol() {
      // EMPTY
    }

    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }

    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
    }
    
    const std::string &name() const {
      return _name;
    }

    const std::string &identifier() const {
      return name();
    }

    std::vector<std::shared_ptr<cdk::basic_type>> args_types() const {
      return _args_types;
    }

    std::shared_ptr<cdk::basic_type> arg_type(int i) const {
      return _args_types.at(i);
    }

    void add_arg_type(std::shared_ptr<cdk::basic_type> type) {
      _args_types.push_back(type);
    }
    
    long value() const {
      return _value;
    }
    
    long value(long v) {
      return _value = v;
    }

    int qualifier() const {
      return _qualifier;
    }

    bool initialized() const {
      return _initialized;
    }

    int offset() const {
      return _offset;
    }

    void set_offset(int offset) {
      _offset = offset;
    }

    bool isFunction() const {
      return _isFunction;
    }

    bool global() const {
      return _offset == 0;
    }

    bool isVariable() const {
      return !_isFunction;
    }

    bool forward() const {
      return _forward;
    }
  };

} // og

#endif
