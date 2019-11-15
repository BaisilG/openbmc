#include <stdlib.h>
#include <stdint.h>
#include "dimm-util.h"

// DIMM manufacturer name as defined in JDEC spec JEP106AV
// https://www.jedec.org/document_search?search_api_views_fulltext=jep106
//
char const *manufacturer_name[] =
{
  "AMD",
  "AMI",
  "N/A",
  "Fujitsu",
  "N/A",
  "N/A",
  "Hitachi",
  "Inmos",
  "N/A",
  "N/A",
  "N/A",
  "Intersil",
  "N/A",
  "Mostek",
  "Freescale (Motorola)",
  "N/A",
  "NEC",
  "N/A",
  "N/A",
  "Conexant (Rockwell)",
  "N/A",
  "NXP (Philips)",
  "Synertek",
  "N/A",
  "N/A",
  "Xicor",
  "Zilog",
  "N/A",
  "Mitsubishi",
  "N/A",
  "N/A",
  "Atmel",
  "STMicroelectronics",
  "N/A",
  "N/A",
  "Wafer Scale Integration",
  "N/A",
  "Tristar",
  "Visic",
  "N/A",
  "N/A",
  "MicrochipTechnology",
  "Ricoh Ltd.",
  "N/A",
  "Micron Technology",
  "N/A",
  "N/A",
  "ACTEL",
  "N/A",
  "Catalyst",
  "Panasonic",
  "N/A",
  "Cypress",
  "N/A",
  "N/A",
  "Zarlink (Plessey)",
  "UTMC",
  "N/A",
  "N/A",
  "Integrated CMOS (Vertex)",
  "N/A",
  "Tektronix",
  "Oracle Corporation",
  "N/A",
  "ProMos/Mosel Vitelic",
  "N/A",
  "N/A",
  "Xerox",
  "N/A",
  "SanDisk Corporation",
  "Elan Circuit Tech",
  "N/A",
  "N/A",
  "Xilinx",
  "Compaq",
  "N/A",
  "SCI",
  "N/A",
  "N/A",
  "I3 Design System",
  "N/A",
  "Crosspoint Solutions",
  "Alliance Semiconductor",
  "N/A",
  "Hewlett-Packard",
  "N/A",
  "N/A",
  "New Media",
  "MHS Electronic",
  "N/A",
  "N/A",
  "Kawasaki Steel",
  "N/A",
  "TECMAR",
  "Exar",
  "N/A",
  "N/A",
  "Northern Telecom",
  "Sanyo",
  "N/A",
  "Crystal Semiconductor",
  "N/A",
  "N/A",
  "Asparix",
  "Convex Computer",
  "N/A",
  "N/A",
  "Transwitch",
  "N/A",
  "Cannon",
  "Altera",
  "N/A",
  "Qualcomm",
  "N/A",
  "N/A",
  "AMS(Austria Micro)",
  "N/A",
  "Aster Electronics",
  "Bay Networks (Synoptic)",
  "N/A",
  "N/A",
  "Thesys",
  "Solbourne Computer",
  "N/A",
  "Dialog Semiconductor",
  "N/A",
  "N/A",
  "N/A",
  "N/A",
  "N/A",
  "N/A",
  "Fairchild",
  "N/A",
  "GTE",
  "Harris",
  "N/A",
  "N/A",
  "Intel",
  "I.T.T.",
  "N/A",
  "Monolithic Memories",
  "N/A",
  "N/A",
  "National",
  "N/A",
  "RCA",
  "Raytheon",
  "N/A",
  "Seeq",
  "N/A",
  "N/A",
  "Texas Instruments",
  "Toshiba",
  "N/A",
  "N/A",
  "Eurotechnique",
  "N/A",
  "Lucent (AT&T)",
  "Exel",
  "N/A",
  "N/A",
  "Lattice Semi.",
  "NCR",
  "N/A",
  "IBM",
  "N/A",
  "N/A",
  "Intl. CMOS Technology",
  "SSSI",
  "N/A",
  "N/A",
  "VLSI",
  "N/A",
  "SK Hynix",
  "OKI Semiconductor",
  "N/A",
  "Sharp",
  "N/A",
  "N/A",
  "IDT",
  "N/A",
  "DEC",
  "LSI Logic",
  "N/A",
  "N/A",
  "Thinking Machine",
  "Thomson CSF",
  "N/A",
  "Honeywell",
  "N/A",
  "N/A",
  "Silicon Storage Technology",
  "N/A",
  "Infineon (Siemens)",
  "Macronix",
  "N/A",
  "Plus Logic",
  "N/A",
  "N/A",
  "European Silicon Str.",
  "Apple Computer",
  "N/A",
  "N/A",
  "Protocol Engines",
  "N/A",
  "Seiko Instruments",
  "Samsung",
  "N/A",
  "Klic",
  "N/A",
  "N/A",
  "Tandem",
  "N/A",
  "Integrated Silicon Solutions",
  "Brooktree",
  "N/A",
  "N/A",
  "Performance Semi.",
  "Winbond Electronic",
  "N/A",
  "Bright Micro",
  "N/A",
  "N/A",
  "PCMCIA",
  "LG Semi (Goldstar)",
  "N/A",
  "N/A",
  "Array Microsystems",
  "N/A",
  "Analog Devices",
  "PMC-Sierra",
  "N/A",
  "N/A",
  "Quality Semiconductor",
  "Nimbus Technology",
  "N/A",
  "Micronas (ITT Intermetall)",
  "N/A",
  "N/A",
  "NEXCOM",
  "N/A",
  "Sony",
  "Cray Research",
  "N/A",
  "Vitesse",
  "N/A",
  "N/A",
  "Zentrum/ZMD",
  "TRW",
  "N/A",
  "N/A",
  "Allied-Signal",
  "N/A",
  "Media Vision",
  "N/A",
  "Numonyx Corporation",
  "N/A",
};

const char * manu_string(uint8_t id)
{
  // val is 4 bit value
  if ((id < 0) ||
      (id >= ARRAY_SIZE(manufacturer_name)) ||
      (manufacturer_name[id] == NULL)) {
    return "Unknown";
  } else {
    return manufacturer_name[id];
  }
}
