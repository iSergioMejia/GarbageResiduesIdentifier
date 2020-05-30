#include "Image.h"
#include <iostream>
#include <vector>

int regiones(float minEnergy, float maxEnergy, float minHomo, float maxHomo, float imgColor, float imgHomo, float imgEnergy);

int main(int argc, char** argv)
{
  std::string name = std::string(argv[1]);
  std::vector<Image> imgs;
	std::string path = "../Imagenes/";
  if(argc == 2){
    Image img(path+name);
    
    imgs[i].printBGRHistogram();
    img.printHSVHistogram();
    imgs[i].printYCrCbHistogram();

  }else{
    int num_imgs = atoi(argv[2]);
    float minEnergy, maxEnergy, minHomo, maxHomo;
    minEnergy = 9999;
    maxEnergy = 0;
    minHomo = 9999;
    maxHomo = 0;

    for(int i = 0; i < num_imgs; i++)
    {
      std::cout << "PROCESANDO" << name+std::to_string(i)+".jpg" << std::endl;
      Image img(path+name+std::to_string(i)+".jpg");
      imgs.push_back(img);
    }
    for(int i = 0; i < num_imgs; i++)
    {
      std::cout << "NAME: " << imgs[i].getName() << std::endl;
      std::cout << "Main Color: " << imgs[i].getMainColor() << std::endl;
      std::cout << "Energy: " << imgs[i].getTextureEnergy() << std::endl;
      std::cout << "Contrast: " << imgs[i].getTextureContrast() << std::endl;
      std::cout << "Homogenity: " << imgs[i].getTextureHomogenity() << std::endl;
      std::cout << "Entropy: " << imgs[i].getTextureEntropy() << std::endl;
      std::cout << "IDM: " << imgs[i].getTextureIDM() << std::endl;
      std::cout << "Mean: " << imgs[i].getTextureMean() << std::endl;
      std::cout << std::endl;

      //std::cout << imgs[i].getMainColor() << "," << imgs[i].getTextureEnergy() << "," << imgs[i].getTextureContrast() << "," << imgs[i].getTextureHomogenity() 
      //<< "," << imgs[i].getTextureEntropy() << "," << imgs[i].getTextureIDM() << "," << imgs[i].getTextureMean() << std::endl;

      if(imgs[i].getTextureEnergy()< minEnergy)
      {
        minEnergy = imgs[i].getTextureEnergy();
      }

      if(imgs[i].getTextureEnergy() > maxEnergy){
        maxEnergy = imgs[i].getTextureEnergy();
      }

      if(imgs[i].getTextureHomogenity() < minHomo){
        minHomo = imgs[i].getTextureHomogenity();
      }

      if(imgs[i].getTextureHomogenity() > maxHomo){
        maxHomo = imgs[i].getTextureHomogenity();
      }


      //std::cout << "Shape " << img[]
      //imgs[i].printBGRHistogram();
      //imgs[i].printHSVHistogram();
      //imgs[i].printYCrCbHistogram();
    }
    for(int i = 0; i < num_imgs; i++)
    {
      std::cout<< "-----------------------------"<<std::endl;
      int region;
      region = regiones (minEnergy, maxEnergy, minHomo, maxHomo, imgs[i].getMainColor(), imgs[i].getTextureHomogenity(), imgs[i].getTextureEnergy() );
      std::cout<< "Imagen "<< imgs[i].getName() <<std::endl;
      if(region == 1 || region == 2){
        std::cout<< "Es un Fruta"<<std::endl;
        std::cout<< "Pertenece a la region "<< region <<std::endl;
      }

      if(region == 3 || region == 4 ||  region == 5 || region == 7 || region == 8){
        std::cout<< "Es un Paquete"<<std::endl;
        std::cout<< "Pertenece a la region "<< region <<std::endl;
      }

      if(region == 9){
        std::cout<< "Es indeterminado"<<std::endl;
        std::cout<< "Pertenece a la region "<< region <<std::endl;
      }

      std::cout<< "Energia: "<< imgs[i].getTextureEnergy()<<std::endl;
      std::cout<< "Homogeneidad: "<< imgs[i].getTextureHomogenity()<<std::endl;
      std::cout<< "Color Principal: "<< imgs[i].getMainColor()<<std::endl;


    }
  }
  
}

int regiones(float minEnergy, float maxEnergy, float minHomo, float maxHomo, float imgColor, float imgHomo, float imgEnergy){

  float conEnergy = minEnergy+(maxEnergy-minEnergy)/3;
  float conEnergy2 = minEnergy+2*(maxEnergy-minEnergy)/3;
  float conEnergy3 = minEnergy+(maxEnergy-minEnergy)/2;

  float conHomo = minHomo+(maxHomo-minHomo)/3;
  float conHomo2 = minHomo+(maxHomo-minHomo)/2;

  if(imgColor >= 0 && imgColor < 150 && imgHomo >= minHomo && imgHomo <= maxHomo && imgEnergy >= minEnergy && imgEnergy < conEnergy){
    return 1;
  }

  if(imgColor >= 0 && imgColor < 150 && imgHomo >= conHomo && imgHomo <= maxHomo && imgEnergy >= conEnergy && imgEnergy < conEnergy2){
    return 2;
  }

  if(imgColor >= 0 && imgColor < 150 && imgHomo >= conHomo && imgHomo <= maxHomo && imgEnergy >= conEnergy2 && imgEnergy <= maxEnergy ){
    return 3;
  }

  if(imgColor >= 150 && imgColor < 250 && imgHomo >= minHomo && imgHomo <  conHomo && imgEnergy >= minEnergy && imgEnergy < conEnergy3){
    return 4;
  }

  if(imgColor >= 150 && imgColor < 250 && imgHomo >= conHomo && imgHomo <= maxHomo && imgEnergy >= minEnergy && imgEnergy < conEnergy3){
    return 5;
  }

  if(imgColor >= 150 && imgColor < 250 && imgHomo >= minHomo && imgHomo < conHomo && imgEnergy >= conEnergy3 && imgEnergy <= maxEnergy){
    return 6;
  }

  if(imgColor >= 150 && imgColor < 250 && imgHomo >= conHomo && imgHomo <= maxHomo && imgEnergy >= conEnergy3 && imgEnergy <= maxEnergy ){
    return 7;
  }

  if(imgColor >= 250 && imgColor <= 360 && imgHomo >= minHomo && imgHomo < conHomo2 && imgEnergy >= minEnergy && imgEnergy <= maxEnergy){
    return 8;
  }

  if(imgColor >= 250 && imgColor <= 360 && imgHomo >= conHomo2 && imgHomo <= maxHomo && imgEnergy >= minEnergy && imgEnergy <= maxEnergy){
    return 9;
  }
}
