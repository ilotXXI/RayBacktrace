#ifndef CIRCULAR_INDEX_H
#define CIRCULAR_INDEX_H

#include <assert.h>

class CircularIndex {
public:
    CircularIndex(int min, int max);
    CircularIndex(int min, int max, int initialIndex);
    
    int toInt() const;
    
    const CircularIndex operator ++(int);
    CircularIndex & operator ++();
    const CircularIndex operator --(int);
    CircularIndex & operator --();
    
    void setBounded(int index);
    void setCirculary(int index);
    
private:
    int         _index;
    const int   _min;
    const int   _max;
};


// ****Implementation.****
inline CircularIndex::CircularIndex(int min, int max)
    : _index(min)
    , _min(min)
    , _max(max)
{
}

inline CircularIndex::CircularIndex(int min, int max, int initialIndex)
    : _index(initialIndex)
    , _min(min)
    , _max(max)
{
    if (_index < _min || _max < _index) {
        assert(false);
        setBounded(_index);
    }
}
    
inline int CircularIndex::toInt() const
{
    return _index;
}

inline const CircularIndex CircularIndex::operator ++(int)
{
    const CircularIndex old(*this);
    ++(*this);
    return old;
}

inline CircularIndex & CircularIndex::operator ++()
{
    _index = (_index < _max) ? (_index + 1) : _min;
    return *this;
}

inline const CircularIndex CircularIndex::operator --(int)
{
    const CircularIndex old(*this);
    --(*this);
    return old;
}

inline CircularIndex & CircularIndex::operator --()
{
    _index = (_index > _min) ? (_index - 1) : _max;
    return *this;
}

inline void CircularIndex::setBounded(int index)
{
    _index = index;
    if (_index < _min)
        index = _min;
    if (_max < _index)
        index = _max;
}

inline void CircularIndex::setCirculary(int index)
{
    _index = index;
    
    const int width = _max - _min + 1;
    while (_index < _min)
        _index += width;
    while (_index > _max)
        _index -= width;
}

#endif
