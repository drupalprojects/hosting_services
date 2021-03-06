#!/bin/bash

# Set command paths.
ECHO=/bin/echo

# Make sure that the parameters are specified.
if [ -z "$1" ]; then
  $ECHO "Usage: $0 <hostname>"
  exit 1
fi

# Get arguments
url=$1

read -p "Test hosting_services resources for $url? (Y/n)" -n 1
if [[ $REPLY =~ ^[Yy]$ ]]
then
  echo -e "\nBegin test of $url\n"
  #-------------------------
  # Test Platform Operations
  #-------------------------
  echo -e "\nTesting platform operations\n"
  echo -e "---------------------------\n"

  echo -e "\nList second, third and fourth platforms\n\n"
  curl -i -X GET -d "http://$url/hosting_platform?row_count=3&offset=1"

# Site test example: curl -i -X POST -d "url=test3.sfyn.office.koumbit.net&data[platform]=619&data[profile]=51&data[client]=851" "http://$url/hosting_site"

  #-----------------------
  # Test Client Operations
  #-----------------------
  echo -e "\nTesting client operations\n"
  echo -e "-------------------------\n"

  echo -e "\nCreate client named Popo\n\n"
  curl -i -X POST -d "name=Popo" "http://$url/hosting_client"

  echo -e "\nRetrieve client named Popo\n\n"
  curl -i -X GET "http://$url/hosting_client/popo.json"

  echo -e "\nChange client Popo's name to Coconut\n\n"
  curl -i -X PUT -d "data[title]=Coconut" "http://$url/hosting_client/popo"

  echo -e "\nRetrieve all sites for client Coconut\n\n"
  curl -i -X POST -d "nid=coconut" "http://$url/hosting_client/sites"

  echo -e "\nDisable all sites for client Coconut\n\n"
  curl -i -X POST -d "nid=coconut" "http://$url/hosting_client/disable_sites"

  echo -e "\nEnable all sites for client Coconut\n\n"
  curl -i -X POST -d "nid=coconut" "http://$url/hosting_client/enable_sites"

  echo -e "\nDelete client Coconut\n\n"
  curl -i -X DELETE "http://$url/hosting_client/coconut"

  # Finish up the testing
  echo -e "\nCompleted test of $url\n"
else
  echo -e "\nAborted test of $url\n"
  exit
fi
