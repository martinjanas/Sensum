#pragma once

class Interface
{
public:
    Interface(void* obj, const char* name)
    {
        m_obj = obj;
        m_name = name;
    }
	
    Interface(const Interface&) = delete;
    Interface& operator=(const Interface&) = delete;
	
    void print() const;
	
    void* m_obj;
    const char* m_name;
};
