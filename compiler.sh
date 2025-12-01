read -r -p "Enter assignment: " user_choice

case "$user_choice" in
    [1])
        source_file="kiriazis_konstantinos_ergasia_1.c"
        ;;
    [2])
        source_file="kiriazis_konstantinos_ergasia_2.c"
        ;;
    [3])
        source_file="kiriazis_konstantinos_ergasia_3.c"
        ;;
    [4])
        source_file="kiriazis_konstantinos_ergasia_4.c"
        ;;
    [5])
        source_file="kiriazis_konstantinos_ergasia_5_optimal_solution.c"
        ;;
    [6])
        source_file="kiriazis_konstantinos_ergasia_6.c"
        ;;

    *)
        echo "Invalid choice. Exiting."
        exit 1
        ;;
esac

# Compile the chosen source file
gcc "$source_file" -o program_out -lm

# Check for successful compilation
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

# Make executable and run
chmod +x program_out
clear
echo "Running $source_file..."
./program_out
