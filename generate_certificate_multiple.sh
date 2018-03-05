#!/bin/bash
folder_count=10;
rsa_bits="2048";
cert_days="81";
cert_out_file="someFileName";
cert_country="SomeCountry";
cert_state="SomeState";
cert_locality="SomeLocality";
cert_organization="SomeOrg";
cert_organization_unit="SomeOU";
cert_domain="example.org";
while [ $folder_count -gt -1 ]; do
host_count=6000;
cert_folder="./cert$folder_count/";
 
if [ -d $cert_folder ]; then
	echo "Certificates will be created in $cert_folder"
else
	echo -e `mkdir $cert_folder`
fi
while [ $host_count -gt -1 ]; do
	echo -e `openssl req -new -newkey rsa:$rsa_bits -days $cert_days -nodes -x509 -subj "/C=$cert_country/ST=$cert_state/L=$cert_locality/O=$cert_organization/OU=$cert_organization_unit/CN=$folder_count$cert_out_file$host_count.$cert_domain" -keyout $cert_folder$folder_count$cert_out_file$host_count.pem -out $cert_folder$folder_count$cert_out_file$host_count.pem`
	host_count=`expr $host_count - 1`
done
	folder_count=`expr $folder_count - 1`
done
echo "All certificates are in place,Start WolfSSL Server now.."

