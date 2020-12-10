#!/bin/bash

echo "Client request"

echo "Generate files to handle a client request."

echo ""

echo "Example:"

echo ""

echo "Request name:"
echo "  protocol_version"

echo ""

echo "  Creates the following files in lib/client_request:"
echo "  - protocol_version.h"
echo "  - protocol_version.c"
echo "  and the following files in lib/server_packet:"
echo "  - protocol_version.h"
echo "  - protocol_version.c"

echo ""

echo "Request name:"
read request_name

request_name_lowercase="${request_name,,}"
request_name_uppercase="${request_name^^}"

arr=(${request_name_lowercase//_/ })
request_name_uppercamelcase=$(printf %s ${arr[@]^})

echo ""

echo "Generating files for $request_name_lowercase"

cp script/template/request.h lib/client_request/$request_name_lowercase.h
cp script/template/request.c lib/client_request/$request_name_lowercase.c

cp script/template/response.h lib/server_packet/$request_name_lowercase.h
cp script/template/response.c lib/server_packet/$request_name_lowercase.c

sed -i "s/REQUEST_UPPERCASE_NAME/$request_name_uppercase/g" lib/client_request/$request_name_lowercase.h
sed -i "s/REQUEST_LOWERCASE_NAME/$request_name_lowercase/g" lib/client_request/$request_name_lowercase.h
sed -i "s/REQUEST_UPPERCAMELCASE_NAME/$request_name_uppercamelcase/g" lib/client_request/$request_name_lowercase.h

sed -i "s/REQUEST_UPPERCASE_NAME/$request_name_uppercase/g" lib/client_request/$request_name_lowercase.c
sed -i "s/REQUEST_LOWERCASE_NAME/$request_name_lowercase/g" lib/client_request/$request_name_lowercase.c
sed -i "s/REQUEST_UPPERCAMELCASE_NAME/$request_name_uppercamelcase/g" lib/client_request/$request_name_lowercase.c

sed -i "s/REQUEST_UPPERCASE_NAME/$request_name_uppercase/g" lib/server_packet/$request_name_lowercase.h
sed -i "s/REQUEST_LOWERCASE_NAME/$request_name_lowercase/g" lib/server_packet/$request_name_lowercase.h
sed -i "s/REQUEST_UPPERCAMELCASE_NAME/$request_name_uppercamelcase/g" lib/server_packet/$request_name_lowercase.h

sed -i "s/REQUEST_UPPERCASE_NAME/$request_name_uppercase/g" lib/server_packet/$request_name_lowercase.c
sed -i "s/REQUEST_LOWERCASE_NAME/$request_name_lowercase/g" lib/server_packet/$request_name_lowercase.c
sed -i "s/REQUEST_UPPERCAMELCASE_NAME/$request_name_uppercamelcase/g" lib/server_packet/$request_name_lowercase.c
