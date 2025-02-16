#!/bin/bash

# Find all message queues owned by the current user
echo "Listing all message queues owned by user $(whoami):"
ipcs -q | grep "$(whoami)"

# Loop through each queue ID and delete it
for queue_id in $(ipcs -q | awk -v user="$(whoami)" '$3 == user {print $2}'); do
    echo "Deleting message queue with ID: $queue_id"
    ipcrm -q "$queue_id"
done

echo "All message queues owned by $(whoami) have been deleted."
