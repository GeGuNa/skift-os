#pragma once

#include "kernel/devices/Device.h"
#include "kernel/devices/LegacyDevice.h"
#include "kernel/drivers/PCSpeaker.h"
#include "kernel/scheduling/Scheduler.h"
#include "kernel/tasking/Task.h"

struct Speaker
{
    int length;
    int frequency;
};

class PCSpeaker : public LegacyDevice
{
private:
    void note(int length, int freq);

public:
    PCSpeaker(DeviceAddress address);

    ~PCSpeaker();

    ResultOr<size_t> write(size64_t offset, const void *buffer, size_t size) override;
};
