#pragma once

// ----------------------------------------------------------------------------

#include "CoreNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------

#define DEFAULT_CONSTUCTIBLE(className) className() = default;

#define NON_DEFAULT_CONSTRUCTIBLE(className) className() = delete;

#define NON_DESTRUCTIBLE(className) ~className() = delete;

#define NON_COPYABLE(className)                                                \
  className(const className&) = delete;                                        \
  className& operator=(const className&) = delete;

#define NON_MOVABLE(className)                                                 \
  className(className&&) = delete;                                             \
  className& operator=(className&&) = delete;

#define NON_COPYABLE_NON_MOVABLE(className)                                    \
  NON_COPYABLE(className)                                                      \
  NON_MOVABLE(className)

#define STATIC_CLASS(className)                                                \
  NON_DEFAULT_CONSTRUCTIBLE(className)                                         \
  NON_DESTRUCTIBLE(className)                                                  \
  NON_COPYABLE_NON_MOVABLE(className)

// ----------------------------------------------------------------------------

END_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------