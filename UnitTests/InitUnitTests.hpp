

#ifndef NEWFILE_HPP
#define	NEWFILE_HPP

#include "TestContainer.hpp"

#include "Adler32Test.hpp"

namespace UnitTests
{
    void initialize()
    {
        TestContainer::getInstance().append(new Adler32Test());
    }
}

#endif