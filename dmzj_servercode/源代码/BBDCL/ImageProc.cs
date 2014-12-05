using System;
using System.Collections.Generic;
using System.Drawing;
using System.Net.Mime;
using System.Text;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.IO;

namespace BBDCL
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    public class ImageProc : IDisposable
    {
        private Image _img;

        public Image Img
        {
            get { return _img; }
            set { _img = value; }
        }

        private Image _ImageResult;

        public Image ImageResult
        {
            get { return _ImageResult; }
            //set { _ImageResult = value; }
        }

        private MemoryStream _ImageStrem;

        public MemoryStream ImageStrem
        {
            get { return _ImageStrem; }
            
        }

       


        //生成缩略图
        public void GenThumbnailImage()
        {
            _ImageResult = Img.GetThumbnailImage(148, 148, null, new IntPtr());
        }

        //public void GenThumbnailImage(int width, int height)
        //{
        //    if (_ImageResult != null) _ImageResult.Dispose();
        //    _ImageResult = Img.GetThumbnailImage(width, height, null, new IntPtr());
        //}

        public void GenThumbnailImage(int destWidth, int destHeight)
        {
            if (_ImageResult != null) _ImageResult.Dispose();
            if (_ImageStrem != null) _ImageStrem.Dispose();
            _ImageStrem = new MemoryStream();
            System.Drawing.Image imgSource = (Image)this.Img.Clone();
            System.Drawing.Imaging.ImageFormat thisFormat = imgSource.RawFormat;
            int sW = 0, sH = 0;
            // 按比例缩放
            int sWidth = imgSource.Width;
            int sHeight = imgSource.Height;

            if (sHeight > destHeight || sWidth > destWidth)
            {
                if ((sWidth * destHeight) > (sHeight * destWidth))
                {
                    sW = destWidth;
                    sH = (destWidth * sHeight) / sWidth;
                }
                else
                {
                    sH = destHeight;
                    sW = (sWidth * destHeight) / sHeight;
                }
            }
            else
            {
                sW = sWidth;
                sH = sHeight;
            }

            Bitmap outBmp = new Bitmap(destWidth, destHeight);
            Graphics g = Graphics.FromImage(outBmp);
            g.Clear(Color.White);

            // 设置画布的描绘质量
            g.CompositingQuality = CompositingQuality.HighQuality;
            g.SmoothingMode = SmoothingMode.HighQuality;
            g.InterpolationMode = InterpolationMode.HighQualityBicubic;

            //g.DrawImage(imgSource, new Rectangle((destWidth - sW) / 2, (destHeight - sH) / 2, sW, sH), 0, 0, imgSource.Width, imgSource.Height, GraphicsUnit.Pixel);
            g.DrawImage(imgSource, new Rectangle(0, 0, sW, sH), 0, 0, imgSource.Width, imgSource.Height, GraphicsUnit.Pixel);
            g.Dispose();

            // 以下代码为保存图片时，设置压缩质量
            EncoderParameters encoderParams = new EncoderParameters();
            long[] quality = new long[1];
            quality[0] = 100;

            EncoderParameter encoderParam = new EncoderParameter(System.Drawing.Imaging.Encoder.Quality, quality);
            encoderParams.Param[0] = encoderParam;

            try
            {
                //获得包含有关内置图像编码解码器的信息的ImageCodecInfo 对象。
                ImageCodecInfo[] arrayICI = ImageCodecInfo.GetImageEncoders();
                ImageCodecInfo jpegICI = null;
                for (int x = 0; x < arrayICI.Length; x++)
                {
                    if (arrayICI[x].FormatDescription.Equals("JPEG"))
                    {
                        jpegICI = arrayICI[x];//设置JPEG编码
                        break;
                    }
                }

                
                    if (jpegICI != null)
                    {

                        outBmp.Save(_ImageStrem, jpegICI, encoderParams);
                    }
                    else
                    {
                        outBmp.Save(_ImageStrem, thisFormat);
                    }
                    this._ImageResult = Image.FromStream(_ImageStrem);
               

                return ;
            }
            catch
            {
                return ;
            }
            finally
            {
                imgSource.Dispose();
                outBmp.Dispose();
            }
        }

        public void Dispose()
        {
            if (_img != null) _img.Dispose();
            if (_ImageResult != null) _ImageResult.Dispose();
            if (_ImageStrem != null) _ImageStrem.Dispose();
        }
    }
}
