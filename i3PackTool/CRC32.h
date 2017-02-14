#ifndef CRC32_h__
#define CRC32_h__

class CRC32 {
public:
	CRC32();
	~CRC32();
	void	Init_CRC32_Table();
	ULONG	Reflect(ULONG ref, char ch);
	int		Get_CRC(unsigned char *buffer, DWORD dwSize);

private:
	ULONG crc32_table[256];
};

#endif // CRC32_h__
