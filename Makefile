CC = g++
BIN = bin

BOOST_INC = -I/home/dev/blogs/c++/boost/boost_1_68_0/build/include
BOOST_LIB = -L/home/dev/blogs/c++/boost/boost_1_68_0/build/lib
BOOST_LIBS = -lboost_program_options -lboost_system -lboost_serialization

CRYPTOPP_INC = -I/home/dev/blogs/c++/cryptopp-CRYPTOPP_7_0_0
CRYPTOPP_LIB = -L/home/dev/blogs/c++/cryptopp-CRYPTOPP_7_0_0
CRYPTOPP_LIBS = -lcryptopp

STORE_H = core-store.hpp
STORE = core-store.cpp

FS_H = core-storage.hpp
FS = core-storage.cpp
OP_H = op-wrapper.hpp
OP = op-wrapper.cpp

INC = include
MAIN = mpwd.cpp

CORE = mpwd-core.o

$(BIN)/mpwd.app: $(INC)/$(STORE_H) $(INC)/$(FS_H) $(INC)/$(OP_H) $(OP) $(FS) $(MAIN) $(BIN)/$(CORE)
	$(CC) -I$(INC) $(BOOST_INC) $(CRYPTOPP_INC) $(OP) $(FS) $(MAIN) $(BOOST_LIB) $(BOOST_LIBS) $(CRYPTOPP_LIB) $(CRYPTOPP_LIBS) $(BIN)/$(CORE) -o $(BIN)/mpwd.app -std=c++11

# compling the core
$(BIN)/$(CORE): $(INC)/$(STORE_H) $(INC)/$(FS_H) $(STORE) $(FS)
	$(CC) -I$(INC) $(BOOST_INC) $(STORE) -o $(BIN)/$(CORE)


#$(BIN)/$(CORE): $(INC)/$(STORE_H) $(STORE)
#	$(CC) -I$(INC) $(BOOST_INC) $(STORE) -o $(BIN)/$(CORE)

