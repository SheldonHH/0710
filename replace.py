import sys
def replace_cn_with_com(file_path):
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            lines = file.readlines()

        modified_lines = [line.replace(".cn", ".com", 1) for line in lines]

        with open(file_path, 'w', encoding='utf-8') as file:
            file.writelines(modified_lines)

        print(f"Replaced '.cn' with '.com' in {file_path}")

    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python replace.py <file_path>")
    else:
        replace_cn_with_com(sys.argv[1])