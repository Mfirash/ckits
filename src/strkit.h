#ifndef strkit_h
#define strkit_h

#include <windows.h>

class strkit
{
private:
    char *_data;
    size_t _size;
    size_t _capacity;
    void reallocate(size_t new_cap)
    {
        HANDLE heap = GetProcessHeap();
        char *new_buffer = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, new_cap + 1);

        if (_data)
        {
            for (size_t i = 0; i < _size; i++)
            {
                new_buffer[i] = _data[i];
            }
            HeapFree(heap, 0, _data);
        }

        _data = new_buffer;
        _capacity = new_cap;
    }

public:
    strkit() : _data(nullptr), _size(0), _capacity(0) {}
    ~strkit()
    {
        if (_data)
            HeapFree(GetProcessHeap(), 0, _data);
    }
    int strsize() const
    {
        return (int)_size;
    }
    int strlength() const
    {
        return (int)_size;
    }
    bool strempty() const
    {
        return _size == 0;
    }
    int strcapacity() const
    {
        return (int)_capacity;
    }
    void strreserve(int n)
    {
        if (n > _capacity)
        {
            reallocate(n);
        }
    }
    void clear()
    {
        _size = 0;
        if (_data)
            _data[0] = '\0';
    }
    void append(char c)
    {
        if (_size + 1 >= _capacity)
        {
            strreserve(_capacity == 0 ? 8 : _capacity * 2);
        }
        _data[_size] = c;
        _size++;
        _data[_size] = '\0';
    }
    const char *c_str() const
    {
        return _data ? _data : "";
    }
};

#endif