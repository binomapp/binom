#ifndef BUFFER_ARRAY_H
#define BUFFER_ARRAY_H

#include "../utils/types.h"
#include "../utils/var_mem_mngr.h"
#include "value.h"

namespace binom {

class BufferArray : VarMemoryManager<BufferArray> {
  friend class VarMemoryManager<BufferArray>;

  union types {
      void* ptr;
      VarType* type;
      byte* bytes;
      types(void* ptr) : ptr(ptr) {}
  } data;

  inline ui64& length() const {return *reinterpret_cast<ui64*>(data.bytes + 1);}

  inline byte*& ptrImpl() {return data.bytes;}
  inline ui64 msizeImpl() const {return 9 + length() * getMemberSize();}
  void* cloneImpl() const;
  void destroyImpl();

  friend class Variable;
  friend class Object;
  friend struct NamedVariable;

  struct _value_7_1 {ui8 val:7;bool sign:1;};
  static ByteArray toChainNumber(ui64 number);
  static ui64 fromChainNumber(ByteArray::iterator& it);
    
  BufferArray(void* buffer) : data(buffer) {}

public:

  typedef ValueIterator iterator;
  typedef const ValueIterator const_iterator;

  BufferArray(VarType type);
  BufferArray(VarType type, size_t count);
  BufferArray(ValType type);
  BufferArray(ValType type, size_t count);

  BufferArray(const std::string_view str);
  BufferArray(const std::u16string_view str);
  BufferArray(const std::u32string_view str);
  BufferArray(const std::wstring_view wstr);

  BufferArray(const std::string str) : BufferArray(std::string_view(str)) {}
  BufferArray(const std::u16string str) : BufferArray(std::u16string_view(str)) {}
  BufferArray(const std::u32string str) : BufferArray(std::u32string_view(str)) {}
  BufferArray(const std::wstring wstr) : BufferArray(std::wstring_view(wstr)) {}

  BufferArray(const char* c_str) : BufferArray(std::string_view(c_str)) {}
  BufferArray(const char16_t* c_str) : BufferArray(std::u16string_view(c_str)) {}
  BufferArray(const char32_t* c_str) : BufferArray(std::u32string_view(c_str)) {}
  BufferArray(const wchar_t* c_wstr) : BufferArray(std::wstring_view(c_wstr)) {}

  BufferArray(size_t size, ui8 value);
  BufferArray(size_t size, ui16 value);
  BufferArray(size_t size, ui32 value);
  BufferArray(size_t size, ui64 value);
  BufferArray(size_t size, i8 value);
  BufferArray(size_t size, i16 value);
  BufferArray(size_t size, i32 value);
  BufferArray(size_t size, i64 value);

  BufferArray(ui8* values, size_t count);
  BufferArray(ui16* values, size_t count);
  BufferArray(ui32* values, size_t count);
  BufferArray(ui64* values, size_t count);
  BufferArray(i8* values, size_t count);
  BufferArray(i16* values, size_t count);
  BufferArray(i32* values, size_t count);
  BufferArray(i64* values, size_t count);
  BufferArray(ValType type, const void* ptr, size_t count);

  BufferArray(ui8arr array);
  BufferArray(ui16arr array);
  BufferArray(ui32arr array);
  BufferArray(ui64arr array);

  BufferArray(i8arr array);
  BufferArray(i16arr array);
  BufferArray(i32arr array);
  BufferArray(i64arr array);

  BufferArray(ByteArray arr);

  BufferArray(const BufferArray& other);
  BufferArray(BufferArray&& other);
  BufferArray(Primitive primitive);

  ~BufferArray() {destroy();}

  inline VarType getType() const {return *data.type;}
  inline ValType getValType() const {return toValueType(getType());}
  inline bool inRange(ui64 index) const {return length() > index;}
  inline bool isEmpty() const {return !length();}
  inline ui64 getMemberCount() const {return *reinterpret_cast<ui64*>(data.bytes + 1);}
  inline ui8 getMemberSize() const {return toSize(getValType());}
  inline ui64 getDataSize() const {return msize() - 9;}
  inline void* getDataPointer() const {return data.bytes + 9;}

  inline bool isByte() const {return getType() == VarType::byte_array;}
  inline bool isWord() const {return getType() == VarType::word_array;}
  inline bool isDWord() const {return getType() == VarType::dword_array;}
  inline bool isQWord() const {return getType() == VarType::qword_array;}

  bool isPrintable() const;
  bool isWPrintable() const;

  ByteArray serialize() const;
  static BufferArray deserialize(ByteArray::iterator& it);
  static ui64 serializedSize(ByteArray::iterator it);


  static inline ui8 getMemberSize(VarType type) {
    switch (type) {
      case VarType::byte_array: return 1;
      case VarType::word_array: return 2;
      case VarType::dword_array: return 4;
      case VarType::qword_array: return 8;
      default: throw Exception(ErrCode::binom_invalid_type);
    }
  }


  inline ValueRef getValue(ui64 index) const {
    if(index >= getMemberCount()) throw Exception(ErrCode::binom_out_of_range, "Out of buffer array range!");
    return begin()[index];
  }

  ValueRef pushBack(ui64 value);
  ValueRef pushBack(i64 value);
  ValueRef pushBack(f64 value);
  iterator pushBack(const BufferArray& other);
  inline ValueRef pushBack(const ValueRef value) {return pushBack(value.asUnsigned());}

  ValueRef pushFront(ui64 value);
  ValueRef pushFront(i64 value);
  ValueRef pushFront(f64 value);
  iterator pushFront(const BufferArray& other);
  inline ValueRef pushFront(const ValueRef value) {return pushFront(value.asUnsigned());}

  ValueRef insert(const ui64 index, const ui64 value);
  ValueRef insert(const ui64 index, const i64 value);
  ValueRef insert(const ui64 index, const f64 value);
  iterator insert(const ui64 index, const BufferArray& other);
  inline ValueRef insert(ui64 index, const ValueRef value) {return insert(index, value.asUnsigned());}

  void popBack(const ui64 n = 1);
  void popFront(const ui64 n = 1);
  void remove(ui64 index, ui64 n = 1);
  BufferArray subarr(ui64 index, ui64 n);

  void clear();

  inline ValueRef operator[](ui64 index) const {return getValue(index);}

  inline BufferArray& operator+=(const ui64 value) {pushBack(value); return *this;}
  inline BufferArray& operator+=(const i64 value) {pushBack(value); return *this;}
  inline BufferArray& operator+=(const f64 value) {pushBack(value); return *this;}
  inline BufferArray& operator+=(const BufferArray& other) {pushBack(other); return *this;}
  inline BufferArray& operator+=(const ValueRef value) {pushBack(value); return *this;}

  BufferArray& operator=(BufferArray other);

  bool operator==(BufferArray other) const;
  inline bool operator!=(BufferArray other) const {return !(*this == std::move(other));}
  bool operator>(BufferArray other) const;
  bool operator<(BufferArray other) const;
  bool operator>=(BufferArray other) const;
  bool operator<=(BufferArray other) const;

  i8 getCompare(BufferArray other) const;

  iterator findMemory(BufferArray data) const;
  iterator findValue(BufferArray data) const;

  iterator begin() const;
  iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

  inline operator std::string() const { return toString(); }
  inline operator std::u16string() const { return toU16String(); }
  inline operator std::u32string() const { return toU32String(); }
  inline operator std::wstring() const { return toWString(); }
  inline operator ByteArray() const { return toByteArray(); }
  inline operator ByteArrayView() const { return toByteArrayView(); }

  std::string toString() const;
  std::string toU8String() const;
  std::u16string toU16String() const;
  std::u32string toU32String() const;
  std::wstring toWString() const;
  ByteArray toByteArray() const;
  ByteArrayView toByteArrayView() const;

  Variable& asVar() {return *reinterpret_cast<Variable*>(this);}
};



}

const binom::BufferArray operator "" _vbfr(const char* c_str, std::size_t);

#endif
