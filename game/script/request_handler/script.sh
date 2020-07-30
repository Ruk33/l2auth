#!/bin/bash

echo "Request handler"

echo ""

echo "Example:"

echo ""

echo "Packet name:"
echo "  protocol_version"

echo ""

echo "  Creates a new folder on lib/request_handler/protocol_version with"
echo "  files: handler.h, handler.c, response.h, response.c, next_handler.h and next_handler.c"

echo ""

echo "Packet name:"
read packet_name

packet_name_lowercase="${packet_name,,}"
packet_name_uppercase="${packet_name^^}"

echo ""

echo "Generating packet handler and response for $packet_name_lowercase"

mkdir -p lib/request_handler/$packet_name_lowercase

cp script/request_handler/template/handler.h lib/request_handler/$packet_name_lowercase
cp script/request_handler/template/handler.c lib/request_handler/$packet_name_lowercase

cp script/request_handler/template/response.h lib/request_handler/$packet_name_lowercase
cp script/request_handler/template/response.c lib/request_handler/$packet_name_lowercase

cp script/request_handler/template/next_handler.h lib/request_handler/$packet_name_lowercase
cp script/request_handler/template/next_handler.c lib/request_handler/$packet_name_lowercase

sed -i "s/PACKET_NAME/$packet_name_uppercase/g" lib/request_handler/$packet_name_lowercase/handler.h
sed -i "s/PACKET_LOWERCASE_NAME/$packet_name_lowercase/g" lib/request_handler/$packet_name_lowercase/handler.h
sed -i "s/PACKET_NAME/$packet_name_uppercase/g" lib/request_handler/$packet_name_lowercase/handler.c
sed -i "s/PACKET_LOWERCASE_NAME/$packet_name_lowercase/g" lib/request_handler/$packet_name_lowercase/handler.c

sed -i "s/PACKET_NAME/$packet_name_uppercase/g" lib/request_handler/$packet_name_lowercase/response.h
sed -i "s/PACKET_LOWERCASE_NAME/$packet_name_lowercase/g" lib/request_handler/$packet_name_lowercase/response.h
sed -i "s/PACKET_NAME/$packet_name_uppercase/g" lib/request_handler/$packet_name_lowercase/response.c
sed -i "s/PACKET_LOWERCASE_NAME/$packet_name_lowercase/g" lib/request_handler/$packet_name_lowercase/response.c

sed -i "s/PACKET_NAME/$packet_name_uppercase/g" lib/request_handler/$packet_name_lowercase/next_handler.h
sed -i "s/PACKET_LOWERCASE_NAME/$packet_name_lowercase/g" lib/request_handler/$packet_name_lowercase/next_handler.h
sed -i "s/PACKET_NAME/$packet_name_uppercase/g" lib/request_handler/$packet_name_lowercase/next_handler.c
sed -i "s/PACKET_LOWERCASE_NAME/$packet_name_lowercase/g" lib/request_handler/$packet_name_lowercase/next_handler.c
