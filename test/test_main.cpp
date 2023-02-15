#include "unity.h"

void setUp(void)
{
    // set stuff up here
    Serial.begin(115200);
    delay(1);
    Serial.println("start Serial");
    Serial2.begin(9600);
    while (!Serial2)
        ;
    delay(1);
}

void tearDown(void)
{
    // clean stuff up here
}

void test_serial()
{

    Serial2.write("p");

    TEST_ASSERT_TRUE(cbuffer_empty(&buff));
    TEST_ASSERT_FALSE(cbuffer_empty(&buff));
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_serial);

    UNITY_END();
}