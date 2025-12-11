#pragma once

#include <functional>
#include <string>

namespace SamplesUtils {
std::string
GetCurrentAppDir(char** argv);

void
InitRandom();
float
NormalizedRand();
int
Rand();

void
SyncForTargetFPS(unsigned fps);

void
BeginFrameStats();
void
LogFrameStats();

void
DoAfter(const std::function<void(void)>& action, const unsigned milliseconds);
void
DoAfterSync(const std::function<void(void)>& action,
            const unsigned milliseconds);
void
PollDeferredActions();

void
PollSingleDeferredAction();
}