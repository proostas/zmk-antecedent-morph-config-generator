# ZMK Antecedent Morph Config Generator

This app facilitates the bulding of dtsi files to complete
specific Antecedent Morph configurations for my Colemator layout.

## Workflow

1. A schema is filled in template.mindnode using MindNode app.
2. The resulting schema is exported by MindNode as a CVS file.
3. ZMK Antecedent Morph Config Generator is launched.
    * Input file is set to point to the previously exported CVS file.
    * Output file is sit to point to the target dtsi file which **will 
      be overwritten**.
    * Press Process.
    * If error occurs, correct it in the schema and start all over
      from step 1.
4. Build your ZMK firmware and upload it to the keyboard.
