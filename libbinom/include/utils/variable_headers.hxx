#ifndef VARIABLE_HEADERS_HXX
#define VARIABLE_HEADERS_HXX

#include "types.hxx"
#include "bits.hxx"
#include "extended_type_traits.hxx"
#include "reverse_iterator.hxx"

namespace binom::priv {

class BitArrayHeader {
  ui64 bit_size = 0;
  size_t capacity = 0;

  BitArrayHeader(const literals::bitarr& bit_array_data);
  BitArrayHeader(const BitArrayHeader& other);

public:
  static BitArrayHeader* create(const literals::bitarr& bit_array_data);
  static BitArrayHeader* copy(const BitArrayHeader* other);
  static size_t calculateByteSize(size_t bit_count) noexcept;
  static BitIterator increaseSize(BitArrayHeader*& header, size_t bit_count);
  static void reduceSize(BitArrayHeader*& header, size_t bit_count);
  static BitIterator insertBits(priv::BitArrayHeader*& header, size_t at, size_t count);
  static void removeBits(priv::BitArrayHeader*& header, size_t at, size_t count);
  static void shrinkToFit(BitArrayHeader*& header);
  static size_t calculateCapacity(size_t bit_count) noexcept;

  size_t getCapacity() const noexcept;
  size_t getBitSize() const noexcept;
  size_t getByteSize() const noexcept;
  Bits* getData() const noexcept;

  template<typename T>
  inline T* getDataAs() const noexcept {
    return reinterpret_cast<T*>(reinterpret_cast<ui8*>(const_cast<BitArrayHeader*>(this)) + sizeof(BitArrayHeader));
  }

  void operator delete(void* ptr);

  BitValueRef operator[](size_t index) const noexcept;

  BitIterator begin() const noexcept;
  BitIterator end() const noexcept;
  BitReverseIterator rbegin() const noexcept;
  BitReverseIterator rend() const noexcept;
};


class BufferArrayHeader {
  size_t size = 0;
  size_t capacity = 0;

  template<typename T>
  BufferArrayHeader(const std::initializer_list<T>& value_list)
    : size(value_list.size() * sizeof(T)), capacity(calculateCapacity(size)) {
    static_assert (std::is_arithmetic_v<T>, "T isn't arithmetic type:"
                                            " as argument requires const std::initializer_list<T>&"
                                            " where assertion std::is_arithmetic<T>::value is true");
    std::memcpy(getData(), value_list.begin(), size);
  }

  BufferArrayHeader(const BufferArrayHeader& other);
  static size_t calculateCapacity(size_t size) noexcept;

public:
  template<typename T>
  static BufferArrayHeader* create(const std::initializer_list<T>& value_list) {
    static_assert (std::is_arithmetic_v<T>, "T isn't arithmetic type:"
                                            " as argument requires const std::initializer_list<T>&"
                                            " where assertion std::is_arithmetic<T>::value is true");
    return new(new byte[ calculateCapacity(value_list.size() * sizeof(T)) ]) BufferArrayHeader(value_list);
  }

  static BufferArrayHeader* copy(const BufferArrayHeader* other);

  void* getData() const;

  template<typename T>
  inline T* getDataAs() const {return const_cast<T*>(reinterpret_cast<const T*>(this + 1));}

  size_t getSize() const noexcept;
  size_t getElementCount(VarBitWidth type) const noexcept;
  size_t getCapacity() const noexcept;

  static void* increaseSize(BufferArrayHeader*& header, VarBitWidth type, size_t count);
  static void reduceSize(BufferArrayHeader*& header, VarBitWidth type, size_t count);
  static void shrinkToFit(BufferArrayHeader*& header);
  static void* insert(BufferArrayHeader*& header, VarBitWidth type, size_t at, size_t count);
  static void remove(BufferArrayHeader*& header, VarBitWidth type, size_t at, size_t count);

  void* get(VarBitWidth type, size_t at) const;

  void* getBeginPtr() const;
  void* getEndPtr(VarBitWidth type) const;
  void* getReverseBeginPtr(VarBitWidth type) const;
  void* getReverseEndPtr(VarBitWidth type) const;

  void operator delete(void* ptr);

};


class ArrayHeader {
  size_t count;
  size_t capacity;

  ArrayHeader(const literals::arr& value_list);
  ArrayHeader(const ArrayHeader& other);

  static size_t calculateCapacity(size_t count) noexcept;
public:
  typedef Variable* Iterator;
  typedef reverse_iterator::ReverseIterator<Variable*> ReverseIterator;

  static ArrayHeader* create(const literals::arr& value_list);
  static ArrayHeader* copy(const ArrayHeader* other);

  size_t getElementCount() const noexcept;
  size_t getCapacity() const noexcept;
  size_t getSize() const noexcept;

  Variable* getData() const;
  static Iterator increaseSize(ArrayHeader*& header, size_t count);
  static void reduceSize(ArrayHeader*& header, size_t count);
  static void popBack(ArrayHeader*& header, size_t count);
  static Iterator insert(ArrayHeader*& header, size_t at, size_t count);
  static void remove(ArrayHeader*& header, size_t at, size_t count);

  Variable operator[](size_t index);

  Iterator begin() const;
  Iterator end() const;
  ReverseIterator rbegin() const;
  ReverseIterator rend() const;

  void operator delete(void* ptr);
};


class SinglyLinkedListHeader {
  struct Node;
  size_t size = 0;
  Node* first = nullptr;
  Node* last = nullptr;
public:
  class Iterator;
  SinglyLinkedListHeader(const literals::sllist& value_list);
  SinglyLinkedListHeader(const SinglyLinkedListHeader& other);
  ~SinglyLinkedListHeader();

  bool isEmpty();

  void clear();

  Variable pushBack(Variable var);
  Iterator pushBack(const literals::sllist& value_list);

  Variable pushFront(Variable var);
  Iterator pushFront(const literals::sllist& value_list);

  Iterator insert(Iterator it, Variable var);
  void remove(Iterator it);

  Iterator begin() const;
  Iterator end() const;
};


class DoublyLinkedListHeader {
  struct Node;
  size_t size = 0;
  Node* first = nullptr;
  Node* last = nullptr;
public:
  class Iterator;
  typedef reverse_iterator::ReverseIterator<Iterator> ReverseIterator;
  DoublyLinkedListHeader(const literals::dllist& value_list);
  DoublyLinkedListHeader(const DoublyLinkedListHeader& other);
  ~DoublyLinkedListHeader();

  bool isEmpty();

  void clear();

  Variable pushBack(Variable var);
  Iterator pushBack(const literals::dllist& value_list);

  Variable pushFront(Variable var);
  Iterator pushFront(const literals::dllist& value_list);

  Iterator insert(Iterator it, Variable var);

  void popBack();
  void popFront();
  void remove(Iterator it);

  Iterator begin() const;
  Iterator end() const;

  ReverseIterator rbegin() const;
  ReverseIterator rend() const;
};

}


#endif // VARIABLE_HEADERS_HXX
