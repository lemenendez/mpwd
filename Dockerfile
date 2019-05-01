FROM ubuntu
RUN apt-get -y update
RUN apt-get -y install build-essential
RUN apt-get -y install wget
RUN apt-get -y install zip

RUN mkdir /usr/boost
WORKDIR /usr/boost
RUN wget https://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.bz2
RUN sha256sum boost_1_68_0.tar.bz2 
RUN tar xvjf boost_1_68_0.tar.bz2 
WORKDIR /usr/boost/boost_1_68_0
RUN ./bootstrap.sh --prefix=build
RUN ./b2 install

RUN mkdir /usr/cryptopp
RUN mkdir /usr/cryptopp/700
WORKDIR /usr/cryptopp
RUN wget https://www.cryptopp.com/cryptopp700.zip
WORKDIR /usr/cryptopp/700
RUN unzip /usr/cryptopp/cryptopp700.zip
RUN make

COPY [".","/usr/src"]
WORKDIR /usr/src
RUN mkdir bin
RUN make
RUN make build-test
RUN export LD_LIBRARY_PATH=/usr/boost/boost_1_68_0/build/lib ; make test
