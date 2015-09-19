#ifndef __String_H__
#define __String_H__

#include "DList.h"

class String {

private:
	char *string;
    unsigned int allocated_memory;

	void alloc(int required_memory);

public:

	String();
	String(unsigned int required_memory);
	String(const char* format, ...);
	String(const String &new_string);
	~String();

	bool operator== (const String &str_to_compare) const;
	bool operator== (const char *str_to_compare) const;
	bool operator!= (const String &str_to_compare) const;
	bool operator!= (const char *str_to_compare) const;
	const String& operator= (const String &str_to_assign);
	const String& operator= (const char *str_to_assign);
	const String& operator+= (const String &str_to_concatenate);
	const String& operator+= (const char *str_to_concatenate);
	const String& prefix(const String &str_to_prefix);
	const String& prefix(const char *str_to_prefix);
	void trim();
	unsigned int find(const char *str_to_find, DList<char*> *start_pos = NULL);
	const String& replace(const String &str_to_substitute, const String &new_string);

	// Ricard method
	const String& create(const char *format, ...);

	unsigned int getLength() const;
	const char* getString() const;
	unsigned int capacity() const;
	void clear();
	
};
#endif // !__String_H__
