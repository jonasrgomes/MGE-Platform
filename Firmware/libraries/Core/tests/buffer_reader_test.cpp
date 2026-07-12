/******************************************************************************
 *
 * MGE Platform
 * BufferReader Tests
 *
 ******************************************************************************/

#include <cassert>
#include <iostream>

#include "mge/core/buffer_reader.h"

using namespace mge::core;

int main()
{
    std::cout << "=====================================\n";
    std::cout << " BufferReader Unit Tests\n";
    std::cout << "=====================================\n";

    //----------------------------------------------------------
    // Teste 1
    //----------------------------------------------------------

    {
        byte data[] =
        {
            0x12,
            0x34,
            0x56,
            0x78
        };

        BufferView view(data,sizeof(data));

        BufferReader reader(view);

        uint32_t value;

        assert(reader.readU32(value));

        assert(value == 0x12345678);

        std::cout << "[OK] Read U32\n";
    }

    //----------------------------------------------------------
    // Teste 2
    //----------------------------------------------------------

    {
        byte data[] =
        {
            0xAB
        };

        BufferView view(data,sizeof(data));

        BufferReader reader(view);

        uint8_t value;

        assert(reader.peekU8(value));

        assert(value==0xAB);

        assert(reader.position()==0);

        std::cout << "[OK] Peek U8\n";
    }

    //----------------------------------------------------------
    // Teste 3
    //----------------------------------------------------------

    {
        byte data[] =
        {
            0x10,
            0x20
        };

        BufferView view(data,sizeof(data));

        BufferReader reader(view);

        uint8_t value;

        reader.readU8(value);

        assert(reader.position()==1);

        reader.readU8(value);

        assert(reader.position()==2);

        assert(reader.eof());

        std::cout << "[OK] Position\n";
    }

    //----------------------------------------------------------
    // Teste 4
    //----------------------------------------------------------

    {
        byte data[] =
        {
            0x11
        };

        BufferView view(data,sizeof(data));

        BufferReader reader(view);

        uint32_t value;

        Result r=reader.readU32(value);

        assert(r.failed());

        std::cout << "[OK] Underflow\n";
    }

    //----------------------------------------------------------
    // Teste 5
    //----------------------------------------------------------

    {
        byte data[] =
        {
            0xAA,
            0xBB,
            0xCC
        };

        BufferView view(data,sizeof(data));

        BufferReader reader(view);

        assert(reader.remaining()==3);

        uint8_t x;

        reader.readU8(x);

        assert(reader.remaining()==2);

        std::cout << "[OK] Remaining\n";
    }

    std::cout << "\n";

    std::cout << "All tests passed.\n";

    return 0;
}