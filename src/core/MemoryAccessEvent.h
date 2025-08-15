// src/core/MemoryAccessEvent.h
/**
 * @file MemoryAccessEvent.h
 * @brief Event object representing a single memory access.
 */
#ifndef MEMORYACCESSEVENT_H
#define MEMORYACCESSEVENT_H

/**
 * @brief Memory access event with optional write flag.
 */
class MemoryAccessEvent {
  int  pageId_;  ///< Virtual page being accessed.
  bool write_;   ///< True if this is a write access.
public:
  /**
   * @brief Construct a memory access event.
   * @param pageId Virtual page ID.
   * @param write True if write access; false for read.
   */
  MemoryAccessEvent(int pageId, bool write=false) : pageId_(pageId), write_(write) {}

  /** @return Virtual page ID. */
  int  pageId()  const { return pageId_; }
  /** @return True if this is a write access. */
  bool write()   const { return write_; }
  /** @return True if this is a write access (compat accessor). */
  bool isWrite() const { return write_; }
};

#endif // MEMORYACCESSEVENT_H
