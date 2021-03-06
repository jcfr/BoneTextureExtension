/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/


// Use an anonymous namespace to keep class types and function names
// from colliding when module is used as shared object module.  Every
// thing should be in an anonymous namespace except for the module
// entry point, e.g. main()
//
#include <fstream>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkFloatingPointExceptions.h"
#include "itkImage.h"
#include "itkVector.h"
#include "itkNeighborhood.h"
#include "itkMetaDataDictionary.h"
#include "itkMetaDataObject.h"

#include "itkVectorIndexSelectionCastImageFilter.h"

#include "itkPluginUtilities.h"

#include "SaveVectorImageAsCSVCLP.h"

namespace
{

template< typename TPixel >
int DoIt( int argc, char * argv[] )
{

    PARSE_ARGS;

    const unsigned int Dimension = 3;

    typedef TPixel                                       PixelType;
    typedef itk::VectorImage< PixelType, Dimension >     InputImageType;
    typedef itk::Image< PixelType, Dimension >           InputMaskType;
    typedef itk::ImageFileReader< InputImageType >       ReaderType;
    typedef itk::ImageFileReader< InputMaskType >        MaskReaderType;

    typename ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( inputVolume );
    reader->Update();

    std::ofstream outputFile;
    const char *outputFilename = outputFileBaseName.c_str();
    outputFile.open(outputFilename, std::ios::out);

    if(predefineTitle)
    {
        outputFile<<"X"<<",";
        outputFile<<"Y"<<",";
        outputFile<<"Z"<<",";
        outputFile<<"Energy"<<",";
        outputFile<<"Entropy"<<",";
        outputFile<<"Correlation"<<",";
        outputFile<<"InverseDifferenceMoment"<<",";
        outputFile<<"Inertia"<<",";
        outputFile<<"ClusterShade"<<",";
        outputFile<<"ClusterProminence"<<",";
        outputFile<<"HarralickCorrelation"<<",";
        outputFile<<"ShortRunEmphasis"<<",";
        outputFile<<"LongRunEmpasis"<<",";
        outputFile<<"GreyLevelNonUniformity"<<",";
        outputFile<<"RunLengthNonUniformity"<<",";
        outputFile<<"LowGreyLevelRunEmphasis"<<",";
        outputFile<<"HighGreyLevelRunEmphasis"<<",";
        outputFile<<"ShortRunLowGreyLevelEmphasis"<<",";
        outputFile<<"ShortRunHighGreyLevelEmphasis"<<",";
        outputFile<<"LongRunLowGreyLevelEmphasis"<<",";
        outputFile<<"LongRunHighGreyLevelEmphasis"<<",";
        outputFile<<"BVTV"<<",";
        outputFile<<"TbN"<<",";
        outputFile<<"TbTh"<<",";
        outputFile<<"TbSp"<<",";
        outputFile<<"BSBV"<<",";


        outputFile<<std::endl;
    }

    typename itk::ImageRegionConstIterator< InputImageType > inIt( reader->GetOutput(),  reader->GetOutput()->GetRequestedRegion());
    inIt.GoToBegin ();
    typename InputImageType::PixelType inputPixel;
    typename InputImageType::IndexType inputIndex;
    const unsigned int VectorComponentDimension = reader->GetOutput()->GetNumberOfComponentsPerPixel();

    if(inputMask != "")
    {
        typename MaskReaderType::Pointer maskReader = MaskReaderType::New();
        maskReader->SetFileName( inputMask );
        maskReader->Update();
        typename itk::ImageRegionConstIterator< InputMaskType > maskIt( maskReader->GetOutput(),  maskReader->GetOutput()->GetRequestedRegion());
        maskIt.GoToBegin ();
        if(secondInputVolume != "")
        {
            typename ReaderType::Pointer secondReader = ReaderType::New();
            secondReader->SetFileName( secondInputVolume );
            secondReader->Update();
            typename itk::ImageRegionConstIterator< InputImageType > secInIt( secondReader->GetOutput(),  secondReader->GetOutput()->GetRequestedRegion());
            secInIt.GoToBegin ();
            const unsigned int SecondVectorComponentDimension = secondReader->GetOutput()->GetNumberOfComponentsPerPixel();

            if(thirdInputVolume != "")
            {
                typename ReaderType::Pointer thirdReader = ReaderType::New();
                thirdReader->SetFileName( thirdInputVolume );
                thirdReader->Update();
                typename itk::ImageRegionConstIterator< InputImageType > thirdInIt( thirdReader->GetOutput(),  thirdReader->GetOutput()->GetRequestedRegion());
                thirdInIt.GoToBegin ();
                const unsigned int ThirdVectorComponentDimension = thirdReader->GetOutput()->GetNumberOfComponentsPerPixel();

                /// Mask + Input Volume + second and third Input Volume ///
                while ( !inIt.IsAtEnd() )
                {
                    if(maskIt.Get() != 0)
                    {
                        inputIndex = inIt.GetIndex();
                        for( unsigned int i = 0; i < Dimension; i++ )
                        {
                            outputFile<<inputIndex[i]<<",";
                        }
                        inputPixel = inIt.Get();
                        for( unsigned int i = 0; i < VectorComponentDimension; i++ )
                        {
                            outputFile<<inputPixel[i]<<",";
                        }
                        inputPixel = secInIt.Get();
                        for( unsigned int i = 0; i < SecondVectorComponentDimension; i++ )
                        {
                            outputFile<<inputPixel[i]<<",";
                        }
                        inputPixel = thirdInIt.Get();
                        for( unsigned int i = 0; i < ThirdVectorComponentDimension; i++ )
                        {
                            outputFile<<inputPixel[i];
                            if (i != (ThirdVectorComponentDimension - 1)) outputFile<<",";
                        }
                        outputFile<<std::endl;
                    }
                    ++secInIt;
                    ++inIt;
                    ++maskIt;
                    ++thirdInIt;
                }
            }

            else
            {
                /// Mask + Input Volume + second Input Volume ///
                while ( !inIt.IsAtEnd() )
                {
                    if(maskIt.Get() != 0)
                    {
                        inputIndex = inIt.GetIndex();
                        for( unsigned int i = 0; i < Dimension; i++ )
                        {
                            outputFile<<inputIndex[i]<<",";
                        }
                        inputPixel = inIt.Get();
                        for( unsigned int i = 0; i < VectorComponentDimension; i++ )
                        {
                            outputFile<<inputPixel[i]<<",";
                        }
                        inputPixel = secInIt.Get();
                        for( unsigned int i = 0; i < SecondVectorComponentDimension; i++ )
                        {
                            outputFile<<inputPixel[i];
                            if (i != (SecondVectorComponentDimension - 1)) outputFile<<",";
                        }
                        outputFile<<std::endl;
                    }
                    ++secInIt;
                    ++inIt;
                    ++maskIt;
                }
            }
        }


        else
        {

            /// Mask + Input Volume///
            while ( !inIt.IsAtEnd() )
            {
                if(maskIt.Get() != 0)
                {
                    inputIndex = inIt.GetIndex();
                    for( unsigned int i = 0; i < Dimension; i++ )
                    {
                        outputFile<<inputIndex[i]<<",";
                    }
                    inputPixel = inIt.Get();
                    for( unsigned int i = 0; i < VectorComponentDimension; i++ )
                    {
                        outputFile<<inputPixel[i];
                        if (i != (VectorComponentDimension - 1)) outputFile<<",";
                    }
                    outputFile<<std::endl;
                }
                ++inIt;
                ++maskIt;
            }

        }
    }
    else
    {
        if(secondInputVolume != "")
        {
            typename ReaderType::Pointer secondReader = ReaderType::New();
            secondReader->SetFileName( secondInputVolume );
            secondReader->Update();
            typename itk::ImageRegionConstIterator< InputImageType > secInIt( secondReader->GetOutput(),  secondReader->GetOutput()->GetRequestedRegion());
            secInIt.GoToBegin ();
            const unsigned int SecondVectorComponentDimension = reader->GetOutput()->GetNumberOfComponentsPerPixel();

            if(thirdInputVolume != "")
            {
                typename ReaderType::Pointer thirdReader = ReaderType::New();
                thirdReader->SetFileName( thirdInputVolume );
                thirdReader->Update();
                typename itk::ImageRegionConstIterator< InputImageType > thirdInIt( thirdReader->GetOutput(),  thirdReader->GetOutput()->GetRequestedRegion());
                thirdInIt.GoToBegin ();
                const unsigned int ThirdVectorComponentDimension = thirdReader->GetOutput()->GetNumberOfComponentsPerPixel();

                /// Input Volume + second and third Input Volume ///
                while ( !inIt.IsAtEnd() )
                {
                    inputIndex = inIt.GetIndex();
                    for( unsigned int i = 0; i < Dimension; i++ )
                    {
                        outputFile<<inputIndex[i]<<",";
                    }
                    inputPixel = inIt.Get();
                    for( unsigned int i = 0; i < VectorComponentDimension; i++ )
                    {
                        outputFile<<inputPixel[i]<<",";
                    }
                    inputPixel = secInIt.Get();
                    for( unsigned int i = 0; i < SecondVectorComponentDimension; i++ )
                    {
                        outputFile<<inputPixel[i]<<",";
                    }
                    inputPixel = thirdInIt.Get();
                    for( unsigned int i = 0; i < ThirdVectorComponentDimension; i++ )
                    {
                        outputFile<<inputPixel[i];
                        if (i != (ThirdVectorComponentDimension - 1)) outputFile<<",";
                    }
                    outputFile<<std::endl;
                    ++secInIt;
                    ++inIt;
                    ++thirdInIt;
                }
            }

            else
            {

                ///Input Volume + second Input Volume ///
                while ( !inIt.IsAtEnd() )
                {

                    inputIndex = inIt.GetIndex();
                    for( unsigned int i = 0; i < Dimension; i++ )
                    {
                        outputFile<<inputIndex[i]<<",";
                    }
                    inputPixel = inIt.Get();
                    for( unsigned int i = 0; i < VectorComponentDimension; i++ )
                    {
                        outputFile<<inputPixel[i]<<",";
                    }
                    inputPixel = secInIt.Get();
                    for( unsigned int i = 0; i < SecondVectorComponentDimension; i++ )
                    {
                        outputFile<<inputPixel[i];
                        if (i != (SecondVectorComponentDimension - 1)) outputFile<<",";
                    }
                    outputFile<<std::endl;
                    ++secInIt;
                    ++inIt;
                }
            }
        }


        else
        {


            ///  Input Volume ///
            while ( !inIt.IsAtEnd() )
            {
                inputIndex = inIt.GetIndex();
                for( unsigned int i = 0; i < Dimension; i++ )
                {
                    outputFile<<inputIndex[i]<<",";
                }
                inputPixel = inIt.Get();
                for( unsigned int i = 0; i < VectorComponentDimension; i++ )
                {
                    outputFile<<inputPixel[i];
                    if (i != (VectorComponentDimension - 1)) outputFile<<",";
                }
                outputFile<<std::endl;
                ++inIt;
            }
        }


    }
    outputFile.close();

    return EXIT_SUCCESS;
}

} // end of anonymous namespace

int main( int argc, char * argv[] )
{
    PARSE_ARGS;

    itk::ImageIOBase::IOPixelType     inputPixelType;
    itk::ImageIOBase::IOComponentType inputComponentType;
    itk::FloatingPointExceptions::Enable();
    itk::FloatingPointExceptions::SetExceptionAction( itk::FloatingPointExceptions::ABORT );

    try
    {

        itk::GetImageType(inputVolume, inputPixelType, inputComponentType);

        switch( inputComponentType )
        {
        case itk::ImageIOBase::UCHAR:
            return DoIt< int >( argc, argv );
            break;
        case itk::ImageIOBase::USHORT:
            return DoIt< int >( argc, argv );
            break;
        case itk::ImageIOBase::SHORT:
            return DoIt< int >( argc, argv );
            break;
        case itk::ImageIOBase::FLOAT:
            return DoIt< float >( argc, argv );
            break;
        case itk::ImageIOBase::INT:
            return DoIt< int >( argc, argv );
            break;
        default:
            std::cerr << "Unknown input image pixel component type: "
                      << itk::ImageIOBase::GetComponentTypeAsString( inputComponentType )
                      << std::endl;
            return EXIT_FAILURE;
            break;
        }
    }
    catch( itk::ExceptionObject & excep )
    {
        std::cerr << argv[0] << ": exception caught !" << std::endl;
        std::cerr << excep << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
