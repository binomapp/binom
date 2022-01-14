#ifndef FILE_NODE_VISITOR_H
#define FILE_NODE_VISITOR_H

#include "file_memory_manager.h"
#include "../utils/node_visitor_base.h"

namespace binom {


class FileNodeVisitor : public NodeVisitorBase {
public:
  class NodeIterator;

  struct ObjectElementPosition {
    ObjectDescriptor descriptor = {};
    ObjectNameLength name_block_descriptor = {};
    real_index name_block_index = 0;
    real_index name_index = 0;
    real_index name_shift = 0;
    real_index index = 0;
  };

private:
  typedef RWSyncMap::ScopedRWGuard ScopedRWGuard;
  typedef RWSyncMap::RWGuard RWGuard;
  typedef RWSyncMap::LockType LockType;
  class ObjectElementFinder;
  static constexpr virtual_index null_index = 0xFFFFFFFFFFFFFFFF;

  struct NamePosition {
    virtual_index parent_node_index = 0;
    ValType char_type = ValType::invalid_type;
    element_cnt length = 0;
    real_index name_pos = 0;
    void setNull() {char_type = ValType::invalid_type;}
    bool isNull() {return char_type == ValType::invalid_type;}
    operator bool () {return !isNull();}
  };


  FileMemoryManager* fmm = nullptr;
  virtual_index node_index = null_index;
  real_index index = null_index;
  NamePosition name_pos;
  mutable RWGuard current_rwg;

  friend class FileNodeVisitor::ObjectElementFinder;
  friend class DynamicStorage;
  friend class FileNodeVisitor::NodeIterator;

  Variable buildVariable(virtual_index node_index) const;
  Primitive buildPrimitive(virtual_index node_index, const NodeDescriptor* descriptor = nullptr, ScopedRWGuard* lk = nullptr) const;
  BufferArray buildBufferArray(virtual_index node_index, const NodeDescriptor* descriptor = nullptr, ScopedRWGuard* lk = nullptr) const;
  Array buildArray(virtual_index node_index, const NodeDescriptor* descriptor = nullptr, ScopedRWGuard* lk = nullptr) const;
  Object buildObject(virtual_index node_index, const NodeDescriptor* descriptor = nullptr, ScopedRWGuard* lk = nullptr) const;

  virtual_index createVariable(Variable variable);
  virtual_index createPrimitive(Primitive primitive);
  virtual_index createBufferArray(BufferArray buffer_array);
  virtual_index createArray(Array array);
  virtual_index createObject(Object object);

  ByteArray getContainedNodeIndexes(virtual_index node_index);
  NodeDescriptor getDescriptor() const;
  NodeFullInfo getFullNodeInfo();
  FileNodeVisitor& setNull();

  inline void throwIfNull() const {if(isNull()) throw Exception(ErrCode::binom_invalid_type, "Try to interact with null!");}

  bool test(Query query, ui64 index) noexcept;

  FileNodeVisitor(FileMemoryManager& fmm, decltype (nullptr))
    : fmm(&fmm), node_index(null_index), index(null_index) {}

  FileNodeVisitor(FileMemoryManager& fmm,
           virtual_index node_index = 0,
           real_index index = null_index)
    : fmm(&fmm),
      node_index(node_index),
      index(index),
      current_rwg(fmm.getRWGuard(node_index))
  {}

  FileNodeVisitor(FileMemoryManager& fmm,
           virtual_index node_index,
           NamePosition name_pos)
    : fmm(&fmm),
      node_index(node_index),
      index(null_index),
      name_pos(name_pos),
      current_rwg(fmm.getRWGuard(node_index))
  {}

public:

  FileNodeVisitor(const FileNodeVisitor& other)
    : fmm(other.fmm),
      node_index(other.node_index),
      index(other.index),
      name_pos(other.name_pos),
      current_rwg(fmm
                  ? fmm->getRWGuard(node_index)
                  : RWGuard())
  {}

  FileNodeVisitor() = default;

  ScopedRWGuard getScopedRWGuard(LockType lock_type = LockType::unlocked) const {throwIfNull(); return ScopedRWGuard(current_rwg, lock_type);}

  inline FileNodeVisitor& operator=(FileNodeVisitor other) {this->~FileNodeVisitor(); return *new(this) FileNodeVisitor(std::move(other));}

  VarType getType() const override;
  VisitorType getVisitorType() const override {throwIfNull(); return VisitorType::file_storage_visitor;}
  inline virtual_index getNodeIndex() const {throwIfNull(); return node_index;}

  inline bool isNull() const override {return !fmm || node_index == null_index;}
  inline bool isValueRef() const override {throwIfNull(); return index != null_index;}
  ui64 getElementCount() const override;
  std::optional<BufferArray> getName();

  FileNodeVisitor& stepInside(ui64 index) override;
  FileNodeVisitor& stepInside(BufferArray name) override;
  FileNodeVisitor& stepInside(Path path) override;

  Variable getVariable() const;
  inline Variable getVariable(ui64 index) const {throwIfNull(); return getChild(index).getVariable();}
  inline Variable getVariable(BufferArray name) const {throwIfNull(); return getChild(name).getVariable();}
  inline Variable getVariable(Path path) const {throwIfNull(); return getChild(path).getVariable();}

  bool contains(ui64 index) override;
  bool contains(BufferArray name) override;
  bool contains(Path path) override;

  void setVariable(Variable var) override;
  void setVariable(ui64 index, Variable var) override;
  void setVariable(BufferArray name, Variable var) override;
  void setVariable(Path path, Variable var) override;

  void pushBack(Variable var) override;
  void pushFront(Variable var) override;

  void insert(ui64 index, Variable var) override;
  void insert(BufferArray name, Variable var) override;

  void remove(ui64 index, ui64 count = 1) override;
  void remove(BufferArray name) override;
  void remove(Path path) override;

  inline FileNodeVisitor getChild(ui64 index) const {throwIfNull(); return FileNodeVisitor(*this)(index);}
  inline FileNodeVisitor getChild(BufferArray name) const {throwIfNull(); return FileNodeVisitor(*this)(name);}
  inline FileNodeVisitor getChild(Path path) const {throwIfNull(); return FileNodeVisitor(*this)(path);}

  NodeVector findSet(Query query, NodeVector node_vector = NodeVector());
  FileNodeVisitor find(Query query);
  FileNodeVisitor findFrom(ui64 index, Query query);
  FileNodeVisitor findFrom(BufferArray name, Query query);
  NodeVector findSetFrom(ui64 index, Query query, NodeVector node_vector = NodeVector());
  NodeVector findSetFrom(BufferArray name, Query query, NodeVector node_vector = NodeVector());

  FileNodeVisitor& operator()(ui64 index) override {throwIfNull(); return stepInside(index);}
  FileNodeVisitor& operator()(BufferArray name) override {throwIfNull(); return stepInside(name);}
  FileNodeVisitor& operator()(Path path) override {throwIfNull(); return stepInside(path);}

  inline FileNodeVisitor operator[](ui64 index) const {throwIfNull(); return getChild(index);}
  inline FileNodeVisitor operator[](BufferArray name) const {throwIfNull(); return getChild(name);}
  inline FileNodeVisitor operator[](Path path) const {throwIfNull(); return getChild(path);}

  operator Variable() override;

  NodeIterator beginFrom(ui64 index);
  NodeIterator beginFrom(BufferArray name);
  NodeIterator begin();
  NodeIterator end();

  NodeVisitor& toRAMVisitor() = delete;
  FileNodeVisitor& toFileVisitor() = delete;
  Variable getVariableClone() = delete;
};

}

#include "file_node_iterator.h"


#endif // FILE_NODE_VISITOR_H
