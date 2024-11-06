#!/bin/bash

# Function to create the address book file
create() {
    echo -e "\nEnter File Name: "
    read filename
    touch "$filename"
    echo "File $filename has been created."
}

# Function to insert a record into the address book
insert() {
    if [ -z "$filename" ]; then
        echo "Error: No address book file created. Please use the 'Create' option first."
        return
    fi
    echo -e "\nEnter Shop Name:"
    read name
    echo "Enter Address:"
    read address
    echo "Enter Shop Id:"
    read shopid
    echo "Name: $name, Address: $address, Shop Id: $shopid" >> $filename
    echo "Insertion Successful!"
}

# Function to display all records in the address book
display() {
    echo "------ DISPLAY ------"
    cat file2.txt
    echo " "
}

# Function to search for a pattern in the address book
search() {
    echo "------ SEARCH ------"
    echo "Enter the pattern to be searched:"
    read pattern
    grep -i "$pattern" file2.txt || echo "No matching records found."
}

# Function to delete a specific line from the address book
delete() {
    echo "-------- DELETE MENU --------"
    echo "Enter line number to delete:"
    read line
    sed -i "${line}d" file2.txt
    echo "Deletion Successful!"
}

# Function to update a specific word in the address book
update() {
    echo "------ UPDATE MENU ------"
    echo "Enter the word that you want to update:"
    read oldValue
    echo "Enter the word that you want to replace it with:"
    read newValue
    sed -i "s/$oldValue/$newValue/g" file2.txt
    echo "Update Successful!"
}

# Main menu loop
while true; do
    echo -e "\nChoose an Option"
    echo -e "1) Create \n2) Insert \n3) Search \n4) Display \n5) Delete \n6) Update \n7) Exit"
    read -p "Enter your choice: " choice
    case "$choice" in
        1) create ;;
        2) insert ;;
        3) search ;;
        4) display ;;
        5) delete ;;
        6) update ;;
        7) exit 0 ;;
        *) echo "Invalid option. Please try again." ;;
    esac
done


#to run
#chmod +x address_book.sh
#./address_book.sh
