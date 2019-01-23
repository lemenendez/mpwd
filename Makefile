CC = g++
BIN = bin

# BOOST
BOOST = /home/dev/blogs/c++/boost/boost_1_68_0/build
BOOST_INC = -I$(BOOST)/include
BOOST_LIB = -L$(BOOST)/lib
BOOST_LIBS = -lboost_program_options -lboost_system -lboost_serialization

# CRYPTOPP
CRYPTOPP = /home/dev/blogs/c++/cryptopp-CRYPTOPP_7_0_0
CRYPTOPP_INC = -I$(CRYPTOPP)
CRYPTOPP_LIB = -L$(CRYPTOPP)
CRYPTOPP_LIBS = -lcryptopp

STORE_H = core-store.hpp
STORE_CPP = core-store.cpp
STORE_O = core-store

STORAGE_H = core-storage.hpp
STORAGE_CPP = core-storage.cpp
STORAGE_O = core-storage

OP_H = op-wrapper.hpp
OP_CPP = op-wrapper.cpp
OP_O =op-wrapper

INC = include
MAIN_CPP = mpwd.cpp
MAIN_O = mpwd.app

#$(BIN)/$(MAIN_O): $(INC)/$(STORE_H) $(INC)/$(FS_H) $(INC)/$(OP_H) $(OP) $(FS) $(MAIN) $(BIN)/$(CORE)
#	$(CC) -I$(INC) $(BOOST_INC) $(CRYPTOPP_INC) $(OP) $(FS) $(MAIN) $(BOOST_LIB) $(BOOST_LIBS) $#(CRYPTOPP_LIB) $(CRYPTOPP_LIBS) $(BIN)/$(CORE) -o $(BIN)/mpwd.app -std=c++11

$(BIN)/$(MAIN_O): $(BIN)/$(OP_O) $(BIN)/$(STORAGE_O) $(BIN)/$(STORE_O) $(MAIN_CPP)
	$(CC) -I$(INC) $(BOOST_INC) $(CRYPTOPP_INC) $(BIN)/$(OP_O) $(BIN)/$(STORAGE_O) $(BIN)/$(STORE_O) $(BOOST_LIB) $(BOOST_LIBS) $(CRYPTOPP_LIB) $(CRYPTOPP_LIBS) $(MAIN_CPP) -o $(BIN)/$(MAIN_O) -std=c++11

$(BIN)/$(STORE_O): $(INC)/$(STORE_H) $(STORE_CPP)
	$(CC) -I$(INC) $(STORE_CPP) -c -o $(BIN)/$(STORE_O)

$(BIN)/$(STORAGE_O): $(INC)/$(STORAGE_H) $(STORAGE_CPP)
	$(CC) -I$(INC) $(CRYPTOPP_INC) $(STORAGE_CPP) -c -o $(BIN)/$(STORAGE_O)

$(BIN)/$(OP_O): $(INC)/$(OP_H) $(OP_CPP)
	$(CC) -I$(INC) $(OP_CPP) -c -o $(BIN)/$(OP_O)

.PHONY: clean
clean:
	rm $(BIN)/$(MAIN_O) $(BIN)/$(OP_O) $(BIN)/$(STORAGE_O) $(BIN)/$(STORE_O)

