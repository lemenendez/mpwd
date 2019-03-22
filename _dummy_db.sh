#!/bin/bash
export LD_LIBRARY_PATH=/home/dev/blogs/c++/boost/boost_1_68_0/build/lib
PROGRAM="./bin/mpwd.app"
FILE="VERY_INSECURE_DB"
PWD="DUMMY_PASSWORD_DUMMY_PASSWORD_DUMMY_PASSWORD_DUMMY_PASSWORD_DUMMY_PASSWORD_DUMMY_PASSWORD"
DEBUG="--encoding none --encrypt none"
USERNAME="jdoe"
EMAIL="john@doe.com"
$PROGRAM --init --f $FILE --p $PWD $DEBUG   # init
$PROGRAM --f $FILE --p $PWD --save --key JibJab --val dummy_pwd --username $EMAIL --url https://example.com --tags service,personal $DEBUG
$PROGRAM --f $FILE --p $PWD --save --key github --val dummy_pwd --username $USER --email $EMAIL --url https://github.com/$USER/ $DEBUG
$PROGRAM --f $FILE --p $PWD --save --key npm --val dummy_pwd --username $USER --url https://www.npmjs.com/ $DEBUG
$PROGRAM --f $FILE --p $PWD --save --key paypal --val dummy_pwd --username $EMAIL --url https://www.paypal.com/us/home $DEBUG
$PROGRAM --f $FILE --p $PWD --search --keyword JibJab --show --history $DEBUG
$PROGRAM --f $FILE --p $PWD --list $DEBUG
$PROGRAM --f $FILE --p $PWD --version