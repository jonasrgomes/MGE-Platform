/******************************************************************************
 *
 * MGE Platform
 * BufferWriter Tests
 *
 ******************************************************************************/

#include <cassert>
#include <iostream>

#include "mge/core/buffer_writer.h"

using namespace mge::core;

int main()
{
    std::cout << "=====================================\n";
    std::cout << " BufferWriter Unit Tests\n";
    std::cout << "=====================================\n";

    //----------------------------------------------------------
    // Teste 1 - Write U8
    //----------------------------------------------------------

    {
        byte buffer[10]{};

        MutableBufferView view(buffer,sizeof(buffer));

        BufferWriter writer(view);

        assert(writer.writeU8(0x55));

        assert(buffer[0]==0x55);

        std::cout << "[OK] Write U8\n";
    }

    //----------------------------------------------------------
    // Teste 2 - Write U16 Big Endian
    //----------------------------------------------------------

    {
        byte buffer[10]{};

        MutableBufferView view(buffer,sizeof(buffer));

        BufferWriter writer(view);

        writer.setEndian(Endian::Big);

        assert(writer.writeU16(0x1234));

        assert(buffer[0]==0x12);
        assert(buffer[1]==0x34);

        std::cout << "[OK] Write U16 Big\n";
    }

    //----------------------------------------------------------
    // Teste 3 - Write U16 Little Endian
    //----------------------------------------------------------

    {
        byte buffer[10]{};

        MutableBufferView view(buffer,sizeof(buffer));

        BufferWriter writer(view);

        writer.setEndian(Endian::Little);

        assert(writer.writeU16(0x1234));

        assert(buffer[0]==0x34);
        assert(buffer[1]==0x12);

        std::cout << "[OK] Write U16 Little\n";
    }

    //----------------------------------------------------------
    // Teste 4 - Position
    //----------------------------------------------------------

    {
        byte buffer[20]{};

        MutableBufferView view(buffer,sizeof(buffer));

        BufferWriter writer(view);

        writer.writeU8(1);

        writer.writeU16(2);

        assert(writer.position()==3);

        std::cout << "[OK] Position\n";
    }

    //----------------------------------------------------------
    // Teste 5 - Remaining
    //----------------------------------------------------------

    {
        byte buffer[20]{};

        MutableBufferView view(buffer,sizeof(buffer));

        BufferWriter writer(view);

        writer.writeU32(10);

        assert(writer.remaining()==16);

        std::cout << "[OK] Remaining\n";
    }

    //----------------------------------------------------------
    // Teste 6 - Overflow
    //----------------------------------------------------------

    {
        byte buffer[2]{};

        MutableBufferView view(buffer,sizeof(buffer));

        BufferWriter writer(view);

        Result r = writer.writeU32(100);

        assert(r.failed());

        std::cout << "[OK] Overflow\n";
    }

    //----------------------------------------------------------
    // Teste 7 - String
    //----------------------------------------------------------

    {
        byte buffer[20]{};

        MutableBufferView view(buffer,sizeof(buffer));

        BufferWriter writer(view);

        assert(writer.writeString("MGE"));

        assert(buffer[0]=='M');
        assert(buffer[1]=='G');
        assert(buffer[2]=='E');
        assert(buffer[3]==0);

        std::cout << "[OK] String\n";
    }

    //----------------------------------------------------------
    // Teste 8 - Float
    //----------------------------------------------------------

    {
        byte buffer[20]{};

        MutableBufferView view(buffer,sizeof(buffer));

        BufferWriter writer(view);

        assert(writer.writeFloat(10.5f));

        std::cout << "[OK] Float\n";
    }

    //----------------------------------------------------------
    // Teste 9 - Double
    //----------------------------------------------------------

    {
        byte buffer[20]{};

        MutableBufferView view(buffer,sizeof(buffer));

        BufferWriter writer(view);

        assert(writer.writeDouble(123.456));

        std::cout << "[OK] Double\n";
    }

    std::cout << "\n";

    std::cout << "All BufferWriter tests passed.\n";

    return 0;
}