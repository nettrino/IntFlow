General
=======

IntFlow is an arithmetic error detection tool that combines static information
flow tracking and dynamic program analysis. By associating sources of untrusted
input with the identified arithmetic errors, IntFlow differentiates between
non-critical, possibly developer-intended undefined arithmetic operations,
and potentially exploitable arithmetic bugs.
\pname examines a broad set of integer errors,
covering almost all cases of C/C++ undefined behaviors, and achieves high
error detection coverage.

You can find all the information on IntFlow's architecture in the following
paper:

"IntFlow: Improving the Accuracy of Arithmetic Error Detection Using Information
Flow Tracking" by Marios Pomonis, Theofilos Petsios, Kangkook Jee, Michalis
Polychronakis, and Angelos D. Keromytis. In Proceedings of the 30th Annual
Computer Security Applications Conference (ACSAC). December 2014, New Orleans,
LA.


Installation
============

See INSTALL.txt for more

License
=======

  Copyright (c) 2014, Columbia University
  All rights reserved.

  This software was developed by (alphabetically) Kangkook Jee, Theofilos Petsios and Marios Pomonis
  at Columbia University, New York, NY, USA, in September 2014.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Columbia University nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
