#include <stdlib.h>
#include <conio.h>
#include<iostream>
#include <chrono>

int main()
{
	long long mean_result = 0;

	for (int t = 0; t < 500; t++)
	{
		int counter = 0;
		int i = 0;
		bool flag = 1;
		FILE* in, * out;
		unsigned __int8* buffer, * pixel_array;
		unsigned __int32 width, height;
		fopen_s(&in, "twilight.bmp", "rb");
		if (in == NULL)
		{
			printf("Error: Can't open twilight.bmp for reading\n");
			return 0;
		}
		buffer = (unsigned __int8*)malloc(54);
		fread(buffer, 1, 54, in);
		for (int i = 0; i <= 3; i++)
		{
			width = buffer[21 - i] | width << 8;
			height = buffer[25 - i] | height << 8;
		}

		pixel_array = (unsigned __int8*)malloc(width * height * 4);
		fread(pixel_array, 1, width * height * 4, in);
		fclose(in);
		int array_size = width * height;

		std::chrono::system_clock::time_point begin = std::chrono::system_clock::now();
		
		
			__asm
			{
				//mov ebx, array_size;
				mov ecx, 1;

				mov ebx, 64646464h;
				movd mm1, ebx;

			M1: //while(i < array_size)
				cmp ecx, array_size;
				jge M2;
				mov eax, counter;//counter++
				inc eax;
				mov counter, eax;

				cmp counter, 64; //if(counter == 256)
				jne M3;
				cmp flag, 1;//(flag = !flag)
				jne M7
					mov flag, 0;
				jmp M8;
			M7:
				mov flag, 1;
			M8:
				mov counter, 0; //counter = 0
			M3:
				cmp flag, 0;//if(flag)
				je M4;
				mov ebx, [pixel_array];
				mov eax, ecx;
				imul eax, 4;
				add ebx, eax;

				movd mm0, dword ptr[ebx];
				paddusb mm0, mm1;// add 100
				movd dword ptr[ebx], mm0;
			M4:
				inc ecx;
				jmp M1;
			M2:
			}
		
		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();

		mean_result += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

		fopen_s(&out, "out.bmp", "wb");
		if (out == NULL)
		{
			printf("Error: Can't open file %s for writing\n", "out.bmp");
			return 0;
		}
		fwrite(buffer, 1, 54, out);
		free(buffer);
		fwrite(pixel_array, 1, width * height * 4, out);
		free(pixel_array);
		fclose(out);
	}

	std::cout << "Mean result = " << mean_result / 500 << " microseconds" << std::endl;

	return 0;
}