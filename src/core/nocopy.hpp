#pragma once

#define CORE_NOCOPY_ASG(type)                                                  \
  type &operator=(type &) = delete;                                            \
  type &operator=(const type &) = delete;

#define CORE_NOCOPY(type)                                                      \
  type(type &)       = delete;                                                 \
  type(const type &) = delete;                                                 \
  CORE_NOCOPY_ASG(type)

#define CORE_NOCOPY_MOVE_1(type)                                               \
  type(type &&) = default;                                                     \
  type &operator=(type &&) = default;

#define CORE_NOCOPY_ASG_MOVE(type)                                             \
  CORE_NOCOPY_ASG(type)                                                        \
  CORE_NOCOPY_MOVE_1(type)

#define CORE_NOCOPY_MOVE(type)                                                 \
  CORE_NOCOPY(type)                                                            \
  CORE_NOCOPY_MOVE_1(type)
