#!/bin/bash

# Exit script on any error
set -e 

#=====================================
# Color Settings:
#=====================================
NC='\033[0m'
OUTPUT='\033[0;32m'
WARNING='\033[0;93m'

echo -e "${OUTPUT}"
echo "=============================================================================="
echo "Running cppcheck"
echo "=============================================================================="
echo -e "${NC}"
echo "Started: $(date)"
echo "Please Wait ..."

# Run cppcheck and output into file
cppcheck  --enable=all \
          -j4 \
          --quiet \
          -i OpenFlipper/libs_required/OpenMesh \
          -i build-release \
          -i build-debug \
          -i .svn \
          -i .git \
          --force \
          -UDOXY_IGNORE_THIS \
          --suppress=unusedPrivateFunction \
          --suppress=ConfigurationNotChecked \
          --suppress=missingIncludeSystem \
          -DOPENMESHDLLEXPORT="" \
          -Umin -Umax -UBMPOSTFIX \
          -DACGDLLEXPORT=""\
          -DDLLEXPORTONLY=""\
          -DDLLEXPORT=""\
          . &> cppcheck.log

echo "Finished: $(date)"

echo -e "${OUTPUT}"
echo "=============================================================================="
echo "CPPCHECK Messages"
echo "=============================================================================="
echo -e "${NC}"


# Echo output to command line for simple analysis via gitlab
cat cppcheck.log

COUNT=$(wc -l < cppcheck.log )

echo -e "${OUTPUT}"
echo "=============================================================================="
echo "CPPCHECK Summary"
echo "=============================================================================="
echo -e "${NC}"

MAX_ERROR=171

if [ $COUNT -gt $MAX_ERROR ]; then
  echo -e ${WARNING}
  echo "Total CPPCHECK error Count is $COUNT, which is too High (Max is $MAX_ERROR)! CPPCHECK Run failed";
  echo -e "${NC}"
  exit 1;
else
  echo "Total CPPCHECK error Count is $COUNT ... OK"
fi

 
