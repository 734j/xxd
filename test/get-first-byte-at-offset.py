#!/usr/bin/python3
def get_byte_at_offset(file_path, offset):
    try:
        with open(file_path, 'rb') as file:
            file.seek(offset)
            byte = file.read(1)
            if byte:
                byte_value = byte[0]
                byte_char = chr(byte_value) if 32 <= byte_value <= 126 else '.'
                byte_hex = format(byte_value, '02X')
                byte_bin = format(byte_value, '08b')
                return byte_char, byte_hex, byte_bin
            else:
                return None
    except Exception as e:
        return f"Error: {e}"

# Example usage
file_path = 'xxd-TESTING'
offset = 0x00193719
result = get_byte_at_offset(file_path, offset)

if result:
    byte_char, byte_hex, byte_bin = result
    print(f"Byte at offset 0x{offset:X}:")
    print(f"Character: {byte_char}")
    print(f"Hex: {byte_hex}")
    print(f"Binary: {byte_bin}")
else:
    print(f"No byte found at offset 0x{offset:X}")
