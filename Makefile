CC = g++
BIN = bin

# BOOST
BOOST = /usr/boost/boost_1_68_0/build
BOOST_INC = -I$(BOOST)/include
BOOST_LIB = -L$(BOOST)/lib
BOOST_LIBS = -lboost_program_options -lboost_system -lboost_serialization

# CRYPTOPP
CRYPTOPP = /usr/cryptopp/700
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
SRC = src

TESTS = tests
TEST_O = test.out

$(BIN)/$(MAIN_O): $(BIN)/$(OP_O) $(BIN)/$(STORE_O) $(BIN)/$(STORAGE_O) $(SRC)/$(MAIN_CPP)
	$(CC) -I$(INC) $(BOOST_INC) $(CRYPTOPP_INC) $(BIN)/$(OP_O) $(BIN)/$(STORAGE_O) $(BIN)/$(STORE_O) $(BOOST_LIB) $(BOOST_LIBS) $(CRYPTOPP_LIB) $(CRYPTOPP_LIBS) $(SRC)/$(MAIN_CPP) -o $(BIN)/$(MAIN_O) -std=c++11

$(BIN)/$(STORE_O): $(INC)/$(STORE_H) $(SRC)/$(STORE_CPP)
	$(CC) -I$(INC) $(BOOST_INC) $(SRC)/$(STORE_CPP) -c -o $(BIN)/$(STORE_O)

$(BIN)/$(STORAGE_O): $(INC)/$(STORAGE_H) $(SRC)/$(STORAGE_CPP)
	$(CC) -I$(INC) $(BOOST_INC) $(CRYPTOPP_INC) $(SRC)/$(STORAGE_CPP) -c -o $(BIN)/$(STORAGE_O)

$(BIN)/$(OP_O): $(INC)/$(OP_H) $(SRC)/$(OP_CPP)
	$(CC) -I$(INC) $(BOOST_INC) $(SRC)/$(OP_CPP) -c -o $(BIN)/$(OP_O)

.PHONY: clean, test, build-test
clean:
	rm $(BIN)/$(MAIN_O) $(BIN)/$(OP_O) $(BIN)/$(STORAGE_O) $(BIN)/$(STORE_O) $(BIN)/$(TEST_O)

test:
	$(BIN)/$(TEST_O)

build-test:
	$(CC) -I$(INC) $(BOOST_INC) $(CRYPTOPP_INC) $(BIN)/$(STORAGE_O) $(BIN)/$(STORE_O) $(BOOST_LIB) $(BOOST_LIBS) $(CRYPTOPP_LIB) $(CRYPTOPP_LIBS) $(TESTS)/test_all.cpp -o $(BIN)/$(TEST_O)

