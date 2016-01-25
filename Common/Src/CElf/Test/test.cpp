#include <string>
#include <iostream>
#include "CElf.h"

using namespace std;

/// \todo Turn this into a Google Test.


// This has enough to tell me I got the code right.
// It's not decoding everything because I'd rather complete the code sooner.
// Maybe I'll finish up later.
void printElf(CElf& myElf)
{
   cout << "ELF File Header\n";
   // e_ident
   cout << "  Class:   " << int(myElf.GetClass()) << endl;
   cout << "  Data:    " << int(myElf.GetDataEncoding()) << endl;
   cout << "  Version: " << int(myElf.GetEIdentVersion()) << endl;
   // other header fields
   cout << "  Type:    " << int(myElf.GetType()) << endl;
   cout << "  Machine: " << int(myElf.GetMachine()) << endl;
   cout << "  Version: " << int(myElf.GetVersion()) << endl;
   cout << "  Entry:   " << int(myElf.GetEntry()) << endl;
   cout << "  Flags:   " << int(myElf.GetFlags()) << endl;
   cout << "  NumSect: " << int(myElf.GetNumSections()) << endl;

   CElf::SectionIterator secIt;
   int i;
   for (i = 0, secIt = myElf.SectionsBegin(); secIt < myElf.SectionsEnd(); secIt++)
   {
      printf("  [ %2d] %-17s SHT             %08x 00off0 %06x ES Flg Lk Inf Al\n",
             i++, (*secIt)->GetName().c_str(), (*secIt)->GetAddress(), (*secIt)->GetData().size());
   }

   CElfSymbolTable *mySt = myElf.GetSymbolTable();

   CElfSymbolTable::SymbolIterator symIt;
   for (i = 0, symIt = mySt->SymbolsBegin(); symIt < mySt->SymbolsEnd(); symIt++, i++)
   {
       string        name;
       unsigned char bind;
       unsigned char type;
       unsigned char other;
       CElfSection*  section;
       Elf32_Addr    value;
       Elf32_Word    size;

       mySt->GetInfo(symIt, &name, &bind, &type, &other, &section, &value, &size);
       printf("%6d: %08x %5d %-10s %s\n", i, value, size,
              section->GetName().c_str(), name.c_str());
   }
}


int main(int argc, char *argv[])
{
   CElf myElf("test.elf");

   printElf(myElf);

   CElfSymbolTable *mySt = myElf.GetSymbolTable();

   // remove a symbol
   mySt->RemoveSymbol("__OpenCL_Copy_fmetadata");

   // remove a section
   myElf.RemoveSection(".amdil");

   // add a section
   CElfSection* myNewSection = myElf.AddSection(
       string(".Teddy"), SHT_PROGBITS, SHF_ALLOC | SHF_EXECINSTR, 0, 4, 0);

   myNewSection->AppendData("Roland Ouellette\0Rebecca Allen\0");

   // add a symbol to the new section
   // value & size make no sense without non-null data...  add error check & then add data.
   mySt->AddSymbol("Aliane", STB_GLOBAL, STT_FUNC, STV_DEFAULT, myNewSection, 0x12, 0x34);

   // add a symbol to an old section
   CElfSection* oldSection = myElf.GetSection(".text");
   mySt->AddSymbol("Grace", STB_GLOBAL, STT_FUNC, STV_DEFAULT, oldSection, 0x56, 0x78);

   printElf(myElf);

   // Write the new file.
   myElf.Store("testout.elf");

   return 0;
}
