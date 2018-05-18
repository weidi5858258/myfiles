//
// Created by root on 18-5-18.
//

#include "./include/MyHead.h"

int read_JPEG_file(string strImageName) {
    /* This struct contains the JPEG decompression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    */
    struct jpeg_decompress_struct cinfo;
    /* We use our private extension JPEG error handler.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */
    struct jpeg_error_mgr jerr;
    /* More stuff */
    FILE *infile;/* source file */
    JSAMPARRAY buffer;/* Output row buffer */
    int row_stride;/* physical row width in output buffer */

    /* In this example we want to open the input file before doing anything else,
    * so that the setjmp() error recovery below can assume the file is open.
    * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
    * requires it in order to read binary files.
    */
    if ((infile = fopen(strImageName.c_str(), "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", strImageName.c_str());
        return -1;
    }

    /* Step 1: allocate and initialize JPEG decompression object */
    /* We set up the normal JPEG error routines, then override error_exit. */
    cinfo.err = jpeg_std_error(&jerr);
    /* Establish the setjmp return context for my_error_exit to use. */
    //if (setjmp(jerr.setjmp_buffer)) {
    /* If we get here, the JPEG code has signaled an error.
    * We need to clean up the JPEG object, close the input file, and return.
    */
    //jpeg_destroy_decompress(&cinfo);
    //fclose(infile);
    //return -1;
    //}

    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);

    /* Step 2: specify data source (eg, a file) */
    jpeg_stdio_src(&cinfo, infile);

    /* Step 3: read file parameters with jpeg_read_header() */
    jpeg_read_header(&cinfo, TRUE);
    /* We can ignore the return value from jpeg_read_header since
    *   (a) suspension is not possible with the stdio data source, and
    *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
    * See libjpeg.txt for more info.
    */
    printf("image_width = %d\n", cinfo.image_width);
    printf("image_height = %d\n", cinfo.image_height);
    printf("num_components = %d\n", cinfo.num_components);

    /* Step 4: set parameters for decompression */
    /* In this example, we don't need to change any of the defaults set by
    * jpeg_read_header(), so we do nothing here.
    */
    printf("enter scale M/N:\n");
    //scanf("%d/%d", &cinfo.scale_num, &cinfo.scale_denom);
    cinfo.scale_num = 1;
    cinfo.scale_denom = 1;
    printf("scale to : %d/%d\n", cinfo.scale_num, cinfo.scale_denom);

    /* Step 5: Start decompressor */
    jpeg_start_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
    * with the stdio data source.
    */

    //输出的图象的信息
    printf("output_width = %d\n", cinfo.output_width);
    printf("output_height = %d\n", cinfo.output_height);
    printf("output_components = %d\n", cinfo.output_components);

    /* We may need to do some setup of our own at this point before reading
    * the data.  After jpeg_start_decompress() we have the correct scaled
    * output image dimensions available, as well as the output colormap
    * if we asked for color quantization.
    * In this example, we need to make an output work buffer of the right size.
    */
    /* JSAMPLEs per row in output buffer */
    row_stride = cinfo.output_width * cinfo.output_components;
    /* Make a one-row-high sample array that will go away when done with image */
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    /* Step 6: while (scan lines remain to be read) */
    /*           jpeg_read_scanlines(...); */
    /* Here we use the library's state variable cinfo.output_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    */
    while (cinfo.output_scanline < cinfo.output_height) {
        /* jpeg_read_scanlines expects an array of pointers to scanlines.
        * Here the array is only one element long, but you could ask for
        * more than one scanline at a time if that's more convenient.
        */
        jpeg_read_scanlines(&cinfo, buffer, 1);
        /* Assume put_scanline_someplace wants a pointer and sample count. */
        //put_scanline_someplace(buffer[0], row_stride);
    }

    /* Step 7: Finish decompression */
    jpeg_finish_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
    * with the stdio data source.
    */

    /* Step 8: Release JPEG decompression object */
    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_decompress(&cinfo);

    /* After finish_decompress, we can close the input file.
    * Here we postpone it until after no more JPEG errors are possible,
    * so as to simplify the setjmp error logic above.  (Actually, I don't
    * think that jpeg_destroy can do an error exit, but why assume anything...)
    */
    fclose(infile);

    /* At this point you may want to check to see whether any corrupt-data
    * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
    */

    return 0;
}

int write_JPEG_file(string strImageName, int quality) {
    unsigned char *image_buffer;    /* Points to large array of R,G,B-order data */
    int image_height = 780; /* Number of rows in image */
    int image_width = 1040;  /* Number of columns in image */

    /* This struct contains the JPEG compression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    * It is possible to have several such structures, representing multiple
    * compression/decompression processes, in existence at once.  We refer
    * to any one struct (and its associated working data) as a "JPEG object".
    */
    struct jpeg_compress_struct cinfo;

    /* This struct represents a JPEG error handler.  It is declared separately
    * because applications often want to supply a specialized error handler
    * (see the second half of this file for an example).  But here we just
    * take the easy way out and use the standard error handler, which will
    * print a message on stderr and call exit() if compression fails.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */
    struct jpeg_error_mgr jerr;
    /* More stuff */
    FILE *outfile;     /* target file */
    JSAMPROW row_pointer[1];    /* pointer to JSAMPLE row[s] */
    int row_stride;     /* physical row width in image buffer */

    /* Step 1: allocate and initialize JPEG compression object */

    /* We have to set up the error handler first, in case the initialization
    * step fails.  (Unlikely, but it could happen if you are out of memory.)
    * This routine fills in the contents of struct jerr, and returns jerr's
    * address which we place into the link field in cinfo.
    */
    cinfo.err = jpeg_std_error(&jerr);
    /* Now we can initialize the JPEG compression object. */
    jpeg_create_compress(&cinfo);

    /* Step 2: specify data destination (eg, a file) */
    /* Note: steps 2 and 3 can be done in either order. */

    /* Here we use the library-supplied code to send compressed data to a
    * stdio stream.  You can also write your own code to do something else.
    * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
    * requires it in order to write binary files.
    */
    if ((outfile = fopen(strImageName.c_str(), "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", strImageName.c_str());
        //exit(1);
        return -1;
    }
    jpeg_stdio_dest(&cinfo, outfile);

    /* Step 3: set parameters for compression */
    /* First we supply a description of the input image.
    * Four fields of the cinfo struct must be filled in:
    */
    cinfo.image_width = image_width;    /* image width and height, in pixels */
    cinfo.image_height = image_height;
    cinfo.input_components = 3;     /* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB;     /* colorspace of input image */
    /* Now use the library's routine to set default compression parameters.
    * (You must set at least cinfo.in_color_space before calling this,
    * since the defaults depend on the source color space.)
    */
    jpeg_set_defaults(&cinfo);
    /* Now you can set any non-default parameters you wish to.
    * Here we just illustrate the use of quality (quantization table) scaling:
    */
    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

    /* Step 4: Start compressor */
    /* TRUE ensures that we will write a complete interchange-JPEG file.
    * Pass TRUE unless you are very sure of what you're doing.
    */
    jpeg_start_compress(&cinfo, TRUE);

    /* Step 5: while (scan lines remain to be written) */
    /*           jpeg_write_scanlines(...); */
    /* Here we use the library's state variable cinfo.next_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    * To keep things simple, we pass one scanline per call; you can pass
    * more if you wish, though.
    */
    row_stride = image_width * 3;   /* JSAMPLEs per row in image_buffer */

    image_buffer = new unsigned char[row_stride * cinfo.image_height];
    memset(image_buffer, 0xff, row_stride * cinfo.image_height);

    unsigned int line = 0;
    //while (cinfo.next_scanline < cinfo.image_height) {
    while (line < cinfo.image_height) {
        /* jpeg_write_scanlines expects an array of pointers to scanlines.
        * Here the array is only one element long, but you could pass
        * more than one scanline at a time if that's more convenient.
        */
        // row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
        row_pointer[0] = &image_buffer[line * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);

        line++;
    }

    delete image_buffer;

    /* Step 6: Finish compression */
    jpeg_finish_compress(&cinfo);
    /* After finish_compress, we can close the output file. */
    fclose(outfile);

    /* Step 7: release JPEG compression object */
    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_compress(&cinfo);

    return 0;
}

struct Image {
    int bpp;
    int width;
    int height;
    unsigned char *data;
};

struct jerror_mgr {
    jpeg_error_mgr base;
    jmp_buf jmp;
};

METHODDEF(void) jerror_exit(j_common_ptr jinfo) {
    jerror_mgr *err = (jerror_mgr *) jinfo->err;
    longjmp(err->jmp, 1);
}

METHODDEF(void) joutput_message(j_common_ptr) {
}

bool Image_LoadJpeg(Image *image, unsigned char *img_data, unsigned int img_size) {
    jpeg_decompress_struct jinfo;
    jerror_mgr jerr;

    jinfo.err = jpeg_std_error(&jerr.base);
    jerr.base.error_exit = jerror_exit;
    jerr.base.output_message = joutput_message;
    jpeg_create_decompress(&jinfo);

    image->data = NULL;

    if (setjmp(jerr.jmp)) goto bail;

    jpeg_mem_src(&jinfo, img_data, img_size);

    if (jpeg_read_header(&jinfo, TRUE) != JPEG_HEADER_OK) goto bail;

    jinfo.dct_method = JDCT_FLOAT; // change this to JDCT_ISLOW on Android/iOS

    if (!jpeg_start_decompress(&jinfo)) goto bail;

    if (jinfo.num_components != 1 && jinfo.num_components != 3) goto bail;

    image->data = new(std::nothrow) unsigned char[jinfo.output_width * jinfo.output_height * jinfo.output_components];
    if (!image->data) goto bail;

    {
        JSAMPROW ptr = image->data;
        while (jinfo.output_scanline < jinfo.output_height) {
            if (jpeg_read_scanlines(&jinfo, &ptr, 1) != 1) goto bail;

            ptr += jinfo.output_width * jinfo.output_components;
        }
    }

    if (!jpeg_finish_decompress(&jinfo)) goto bail;

    image->bpp = jinfo.output_components;
    image->width = jinfo.output_width;
    image->height = jinfo.output_height;

    jpeg_destroy_decompress(&jinfo);

    return true;

    bail:
    jpeg_destroy_decompress(&jinfo);
    if (image->data) delete[] image->data;

    return false;
}

struct ImageData {
    unsigned char *pixels;
    long width;
    long height;
};

int TestImage(string strSrcImageName, string strDstImageName) {
    //read
    struct jpeg_decompress_struct cinfo_decompress;
    FILE *infile;
    int row_stride;
    struct jpeg_error_mgr jerr;

    if ((infile = fopen(strSrcImageName.c_str(), "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", strSrcImageName.c_str());
        return -1;
    }

    cinfo_decompress.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo_decompress);
    jpeg_stdio_src(&cinfo_decompress, infile);
    int ret = jpeg_read_header(&cinfo_decompress, TRUE);
    if (ret != JPEG_HEADER_OK) return -1;
    jpeg_start_decompress(&cinfo_decompress);
    row_stride = cinfo_decompress.output_width * cinfo_decompress.output_components;
    int buffer_height = 1;
    JSAMPARRAY buffer = (JSAMPARRAY) malloc(sizeof(JSAMPROW) * buffer_height);
    buffer[0] = (JSAMPROW) malloc(sizeof(JSAMPLE) * row_stride);
    //JSAMPARRAY buffer = (*cinfo_decompress.mem->alloc_sarray)((j_common_ptr)&cinfo_decompress, JPOOL_IMAGE, row_stride, 1);
    ImageData *imageData;
    imageData = new ImageData;
    imageData->width = cinfo_decompress.output_width;
    imageData->height = cinfo_decompress.output_height;

    imageData->pixels = new unsigned char[cinfo_decompress.output_width * cinfo_decompress.output_height *
                                          cinfo_decompress.output_components];
    long counter = 0;

    while (cinfo_decompress.output_scanline < cinfo_decompress.output_height) {
        jpeg_read_scanlines(&cinfo_decompress, buffer, 1);
        memcpy(imageData->pixels + counter, buffer[0], row_stride);
        counter += row_stride;
    }

    jpeg_finish_decompress(&cinfo_decompress);
    jpeg_destroy_decompress(&cinfo_decompress);

    fclose(infile);

    //write
    unsigned char *image_buffer;
    int image_height = cinfo_decompress.output_height;
    int image_width = cinfo_decompress.output_width;
    FILE *outfile;
    JSAMPROW row_pointer[1];
    int row_stride_dst;
    struct jpeg_compress_struct cinfo_compress;
    cinfo_compress.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo_compress);

    if ((outfile = fopen(strDstImageName.c_str(), "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", strDstImageName.c_str());
        //exit(1);
        return -1;
    }

    jpeg_stdio_dest(&cinfo_compress, outfile);

    cinfo_compress.image_width = image_width;
    cinfo_compress.image_height = image_height;
    cinfo_compress.input_components = 3;
    cinfo_compress.in_color_space = JCS_EXT_RGB;

    int quality = 70;
    jpeg_set_defaults(&cinfo_compress);
    jpeg_set_quality(&cinfo_compress, quality, TRUE);
    jpeg_start_compress(&cinfo_compress, TRUE);

    row_stride_dst = image_width * 3;

    image_buffer = new unsigned char[row_stride_dst * cinfo_compress.image_height];
    memcpy(image_buffer, imageData->pixels, row_stride_dst * cinfo_compress.image_height);

    while (cinfo_compress.next_scanline < cinfo_compress.image_height) {
        row_pointer[0] = &image_buffer[cinfo_compress.next_scanline * row_stride_dst];
        jpeg_write_scanlines(&cinfo_compress, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo_compress);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo_compress);

    if (imageData) {
        delete imageData;
        imageData = NULL;
    }

    if (image_buffer)
        delete[] image_buffer;

    return 0;
}

int main(int argc, char *argv[]) {
    string strImageName = "data/srcImg/moon.jpg";
    int flag1 = read_JPEG_file(strImageName);
    if (flag1 == 0) cout << "read ok!" << endl;
    else cout << "read error!" << endl;

    strImageName = "2.bmp";
    int flag2 = write_JPEG_file(strImageName, 80);
    if (flag2 == 0) cout << "write ok!" << endl;
    else cout << "write error!" << endl;

    string strSrcImageName = "data/srcImg/moon.jpg";
    string strDstImageName = "b.jpg";
    int flag3 = TestImage(strSrcImageName, strDstImageName);
    if (flag3 == 0) cout << "test ok!" << endl;
    else cout << "test error!" << endl;

    return 0;
}