#!/bin/bash

echo "Packet response builder"

echo ""

echo "Example:"

echo ""

echo "Packet name:"
echo "  my_target_selected"
echo "Folder:"
echo "  action"

echo ""

echo "  Creates a new file in lib/request_handler/action/my_target_selected_response.h "
echo "  and lib/request_handler/action/my_target_selected_response.c"

echo ""

echo "Packet name:"
read packet_name

echo ""

echo "Folder:"
read folder

packet_name_lowercase="${packet_name,,}"
packet_name_uppercase="${packet_name^^}"

echo ""

echo "Generating packet response in request_handler/$folder"

mkdir -p lib/request_handler/$folder

cp script/packet_response/template/response.h "lib/request_handler/$folder/${packet_name_lowercase}_response.h"
cp script/packet_response/template/response.c "lib/request_handler/$folder/${packet_name_lowercase}_response.c"

sed -i "s/PACKET_NAME/$packet_name_uppercase/g" "lib/request_handler/$folder/${packet_name_lowercase}_response.h"
sed -i "s/PACKET_LOWERCASE_NAME/$packet_name_lowercase/g" "lib/request_handler/$folder/${packet_name_lowercase}_response.h"
sed -i "s/PACKET_NAME/$packet_name_uppercase/g" "lib/request_handler/$folder/${packet_name_lowercase}_response.c"
sed -i "s/PACKET_LOWERCASE_NAME/$packet_name_lowercase/g" "lib/request_handler/$folder/${packet_name_lowercase}_response.c"
