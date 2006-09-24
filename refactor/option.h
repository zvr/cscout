/*
 * (C) Copyright 2006 Diomidis Spinellis.
 *
 * A user interface option
 *
 * $Id: option.h,v 1.1 2006/09/24 22:08:34 dds Exp $
 */

class BoolOption;
class IntegerOption;
class SelectionOption;
class TextOption;

class Option {
protected:
	const char *short_name;				// File token
	const char *user_name;				// User-interface description
	static vector<Option *> options;		// Options in the order they were added
	static map<string, Option *> omap;	// For loading options
public:
	// Constructor
	Option(const char *sn, const char *un) : short_name(sn), user_name(un) {}
	virtual ~Option() {}

	// Save to a file
	virtual void save(ofstream &ofs) const = 0;
	// Load from a file
	virtual void load(ifstream &ifs) = 0;
	// Set from a submitted page
	virtual void set() = 0;
	// Display on a web page
	virtual void display(FILE *f) = 0;

	// Add a new option to the pool
	static void add(Option *o);
	// Save to a file
	static void save_all(ofstream &ofs);
	// Load from a file
	static void load_all(ifstream &ifs);
	// Set from a submitted page
	static void set_all();
	// Display on a web page
	static void display_all(FILE *f);

	// Global Web options
	static BoolOption *fname_in_context;		// Remove common file prefix
	static BoolOption *show_true;			// Only show true identifier properties
	static BoolOption *show_line_number;		// Annotate source with line numbers
	static BoolOption *file_icase;			// File name case-insensitive match
	static BoolOption *sort_rev;			// Reverse sorting of query results
	static IntegerOption *tab_width;		// Tab width for code output
	static SelectionOption *cgraph_type;		// Call graph type t(text h(tml d(ot s(vg g(if
	static SelectionOption *cgraph_show;		// Call graph show e(dge n(ame f(ile p(ath
	static TextOption *sfile_re_string;		// Saved files replacement location RE string
	static TextOption *sfile_repl_string;		// Saved files replacement string
	static IntegerOption *entries_per_page;		// Number of elements to show in a page
	// Initialize the global web options
	static void initialize();
};

// A boolean value
class BoolOption : public Option {
private:
	bool	v;		// The value
public:
	// Constructor
	BoolOption(const char *sn, const char *un, bool iv = false) : Option(sn, un), v(iv) {}
	// Save to a file
	void save(ofstream &ofs) const { ofs << v; }
	// Load from a file
	void load(ifstream &ifs) { ifs >> v; }
	// Set from a submitted page
	void set() { v = !!swill_getvar(short_name); }
	// Set from a submitted page
	bool get() { return v; }
	// Display on a web page
	void display(FILE *f) {
		fprintf(f,
			"<tr>"
			"<td>%s</td>\n"
			"<td><input type=\"checkbox\" name=\"%s\" value=\"1\" %s></td>"
			"</tr>\n",
			user_name,
			short_name,
			v ? "checked" : ""
		);
	}
};

// A radion selection value represented through a character code
class SelectionOption : public Option {
private:
	char	v;		// The value
	struct SelectionElement {
		char c;
		const char *name;
		SelectionElement(const char *s) : c(*s), name(s + 2) { assert(strlen(s) > 2 && s[1] == ':'); }
	} ;
	vector <SelectionElement> options;
public:
	/*
	 * Construct from name, short name, initial value, and a NULL-terminated list
	 * list of arguments of the form "letter:Description"
	 */
	SelectionOption(const char *sn, const char *un, char iv, ...);
	// Save to a file
	void save(ofstream &ofs) const { ofs << v; }
	// Load from a file
	void load(ifstream &ifs) { ifs >> v; }
	// Set from a submitted page
	void set() {
		char *m;

		if ((m = swill_getvar(short_name)))
			v = *m;
	}
	// Return the value
	char get() { return v; }
	// Display on a web page
	void display(FILE *f);
};

// A textual value
class TextOption : public Option {
private:
	string	v;		// The value
public:
	// Constructor
	TextOption(const char *sn, const char *un, const string &iv = "") : Option(sn, un), v(iv) {}
	// Save to a file
	void save(ofstream &ofs) const { ofs << v; }
	// Load from a file
	void load(ifstream &ifs);
	// Set from a submitted page
	void set() { v = string(swill_getvar(short_name)); }
	// Erase the value
	void erase() { v.erase(); }
	// Return the value
	const string &get() { return v; }
	// Display on a web page
	void display(FILE *f) {
		fprintf(f,
			"<tr>"
			"<td>%s</td>\n"
			"<td><input type=\"text\" name=\"%s\" size=\"20\" maxlength=\"200\" value=\"%s\"></td>"
			"</tr>\n",
			user_name,
			short_name,
			v.c_str()
		);
	}
};

// A (positive) integer value
class IntegerOption : public Option {
private:
	int	v;		// The value
public:
	// Constructor
	IntegerOption(const char *sn, const char *un, int iv) : Option(sn, un), v(iv) {}
	// Save to a file
	void save(ofstream &ofs) const { ofs << v; }
	// Load from a file
	void load(ifstream &ifs) { ifs >> v; }
	// Set from a submitted page
	void set() {
		string arg = string("I(") + short_name + ")";
		int vt;

		if (swill_getargs(arg.c_str(), &vt) && vt > 0)
			v = vt;
	}
	// Return the value
	int get() { return v; }
	// Display on a web page
	void display(FILE *f) {
		fprintf(f,
			"<tr>"
			"<td>%s</td>\n"
			"<td><input type=\"text\" name=\"%s\" size=\"5\" maxlength=\"5\" value=\"%d\"></td>"
			"</tr>\n",
			user_name,
			short_name,
			v
		);
	}
};
