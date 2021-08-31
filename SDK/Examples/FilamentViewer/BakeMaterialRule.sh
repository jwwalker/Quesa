#!/bin/sh

#  BakeMaterialRule.sh
#  FilamentViewer
#
#  Created by James Walker on 6/12/21.
#  

echo "Baking ${INPUT_FILE_BASE}"

"${FilamentRelease}/bin/matc" --api ${MATERIAL_BACKEND} --platform desktop --variant-filter=skinning --output "${SCRIPT_OUTPUT_FILE_0}" "${INPUT_FILE_PATH}"
