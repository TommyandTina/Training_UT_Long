/***********************************************************************************************************************
* Copyright [2021] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
*
* The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
* and/or its licensors ("Renesas") and subject to statutory and contractual protections.
*
* Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
* display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
* purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
* SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
* NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
* INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
* be subject to different terms.
***********************************************************************************************************************/
#include "gtest/gtest.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <devctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/neutrino.h>
#include "thermal.h"

class QNXBSPTest : public ::testing::Test
{
protected:

    void SetUp() override{
        EXPECT_NE(system("rcar-thermal &"), -1);
        delay(500);

    }
    void TearDown() override{
        system("slay rcar-thermal");
    }
};

TEST_F(QNXBSPTest, thermal)
{
    int fd;
    int status;
const char      *devname = DEV_NAME;
    thermal_packet_t data;

    fd = open(devname, O_RDWR);

    ASSERT_GE(fd, 0);

    memset((void *)&data, 0, sizeof(thermal_packet_t));

    status = devctl( fd, DCMD_THSCIVM_GET_TEMPERATURE, &data, sizeof(data), NULL);

    ASSERT_EQ(status, EOK);

    close(fd);
}

#if 0
TEST_F(QNXBSPTest, voltage)
{
    int fd;
    int status;
const char      *devname = DEV_NAME;
    thermal_packet_t data;

    fd = open(devname, O_RDWR);

    ASSERT_GE(fd, 0);

    memset((void *)&data, 0, sizeof(thermal_packet_t));
    status = devctl( fd, DCMD_THSCIVM_GET_VOLTAGE, &data, sizeof(data), NULL);

    ASSERT_EQ(status, EOK);
    close(fd);
}
#endif
