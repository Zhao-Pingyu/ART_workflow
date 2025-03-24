from ovito.io import import_file #, export_file
from ovito.modifiers import ExpressionSelectionModifier, DeleteSelectedModifier, SelectTypeModifier, PolyhedralTemplateMatchingModifier
import numpy as np

pipeline = import_file("9775-1048.data")

pipeline.modifiers.append(PolyhedralTemplateMatchingModifier())
#pipeline.modifiers.append(SelectTypeModifier(operate_on="particles",property="Structure Type", types = {PolyhedralTemplateMatchingModifier.Type.FCC}))

pipeline.modifiers.append(ExpressionSelectionModifier(expression = 'StructureType==1 || abs(Position.Y)>7'))
pipeline.modifiers.append(DeleteSelectedModifier())

data = pipeline.compute()

ids = np.array(data.particles['Particle Identifier'])

#print(ids)
#print(len(ids))
np.savetxt('gb_id.txt',ids.astype(int),fmt='%i')
