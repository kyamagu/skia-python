Since m130:

- We now build for Mac OS 13 (12 deprecated at github), and support python 3.13

- GL examples and tests updated, to restrict "Mac OS friendly" GL settings to Mac only.
  Some Linux installations - specifically, inside github CI - are sensitive to those;
  typical desktop linux boxes with real graphic cards seem not to be affected.

- `PDF.StructureElementNode.fAdditionalNodeIds` withdrawn.
  `SkPDF::StructureElementNode::fAdditionalNodeIds` removed upstream.
