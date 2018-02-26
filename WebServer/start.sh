#! /bin/bash
#
# Usage:  ./start.sh [port]
#  
# Start the service as a background process, saving
# the process number (of the lead process) in SERVICE_PID.
#
#

PORTNUM=$1
if [[ "${PORTNUM}" == "" ]]; then
    PORTNUM="5000"
fi;

echo "***Will listen on port ${PORTNUM}***"

./main.exe ${PORTNUM}

echo "***"
echo "Simple web server started"
echo "Listening on port ${PORTNUM}"
echo "***"