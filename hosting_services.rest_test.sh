#!/bin/bash

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

  echo -e "\nList all platforms\n\n"
  curl -i -X GET -d "row_count=10&offset=1" "http://$url/hosting_platform"

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
