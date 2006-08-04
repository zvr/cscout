/*
 * (C) Copyright 2003 Diomidis Spinellis.
 *
 * Function call graph information
 *
 * $Id: fcall.h,v 1.13 2006/08/04 19:47:52 dds Exp $
 */

#ifndef FCALL_
#define FCALL_

/*
 * Function call information is always associated with Id objects
 * It is thus guaranteed to match the symbol table lookup operations
 */

// C function calling information
class FCall : public Call {
private:
	Tokid definition;		// Function's definition
	Type type;			// Function's type
	bool defined;			// True if the function has been defined
	static stack<FCall *> nesting;	// Nested function definitions
public:
	// Called when outside a function body scope
	static void unset_current_fun();
	// Set the function currently being parsed
	static void set_current_fun(const Type &t);
	static void set_current_fun(const Id *id);

	virtual Tokid get_definition() const { return definition; }
	virtual bool is_defined() const { return defined; }
	virtual bool is_declared() const { return true; }
	virtual bool is_file_scoped() const { return type.is_static(); }
	virtual bool is_cfun() const { return true; }
	virtual bool is_macro() const { return false; }
	virtual const string & entity_type_name() const {
		static string s("C function");
		return (s);
	}
	FCall(const Token& t, Type typ, const string &s);

	virtual ~FCall() {}
};

#endif // FCALL_
