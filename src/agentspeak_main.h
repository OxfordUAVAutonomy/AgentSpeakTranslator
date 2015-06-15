/*
This file is part of AgentSpeakTranslator.

Copyright (c) 2015, Samuel Bucheli, Department of Computer Science, University
of Oxford. All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// NOTE: This file does not contain any code, it's just for the Doxygen mainpage.

/**
 * @mainpage
 *
 * @par Further Information
 * - @subpage example Gives an idea of the AgentSpeak language and what additional steps are necessary in order to use the translator
 * - @subpage overview Gives an overview of the translator code.
 * 
 * @copyright Copyright (c) 2015, Samuel Bucheli, Department of Computer Science, University of Oxford. All rights reserved.
 *
 * @license This project is released under the BSD 2-Clause License.
 *
@verbatim
Copyright (c) 2015, Samuel Bucheli, Department of Computer Science, University
of Oxford. All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
@endverbatim
 *
 * 
@page example Example Walk-Through

\TODO The text below contains probably a lot of typos, my apologies in advance...

The idea of this section is to give an idea of the AgentSpeak language and what additional steps are necessary in order to use the translator. We implement a simplified autopilot for an unmanned aerial vehicle (UAV) as an example. The complete code including a simple simulation environment can be found in <a href="https://github.com/OxfordUAVAutonomy/AgentSpeakTranslator/test/simple_autopilot/">test/simple_autopilot/</a>. The example can be uilt using @c make from the Makefile provided there.

First, we write the actual AgentSpeak code in @c autopilot.asl. Let us start with some very easy behaviour, taking off. Here, we assume that our UAV has some built-in take-off method, which we can access via @c sendTakeOff(). We can implement this behaviour by giving a plan reacting to the triggering event ''achieve takeOff''. One important precondition for taking off is that we have not already taken off.  So, the first line of the following code can be read as ''in case the goal @c takeOff is to be achieved and if @c takenOff is not true, execute the following steps''. 

 * @snippet autopilot.asl takeOff

These steps, called the body of the plan, implement the actual behaviour, which is the following three steps:
-# provide some output to the user using @c notifyUser (some output function provided by the run-time environment),
-# do the actual take-off using @c sendTakeOff,
-# add the belief @c takenOff to the belief base.

Another simple case is landing, which is very similar to the case above. Here we use @c -takenOff to remove the belief from the belief base.
 * @snippet autopilot.asl land

The case of moving the UAV to some given target is more interesting. In order to deal with an event ''go to the given target'', we need two plans. Both plans have in common, that they compare the UAV's current position to the given target and check whether they are close enough (within some radius) to the given target or not. If the UAV is not close enough yet, we need to move the UAV closer to the target using an adequate control command and then try to achieve the goal ''go to the given target'' again. If the UAV is close enough, we simply can issue a corresponding notification to the user. This behaviour is implemented as follows:

 * @snippet autopilot.asl goto

Here we assume that the UAV's current position is stored in the belief base in @c myPosition and that the tolerance radius is stored in @c tolerance. We will address the question of how these values get into the belief base further below. Furthermore, we assume that the environment provides the functions @c closeEnough, @c calculateMovement and @c sendControl. One last, but very important assumption, is that events are handled in a run-to-completion fashion, including any events generated in the course of event handling. In particular, this guarantees that the subsequently generated @c !goto events are immedietaly handled, thus effectively running these plans until the target is actually reached.

Now, the autopilot is only done, but we are missing one crucial behaviour: If the UAV is in the air, but the user provides no instructions, i.e., neither @c goto nor @c land, it is the autopilot's responsibility to keep the UAV in the air. We implement this by providing plans for the event ''wait for commands''. These look as follows:

 * @snippet autopilot.asl waitForCommand

While these look very similar to previously presented plans, there is one important difference to notice here: we use new focus achievement goals @c !!waitForCommand instead of the ''regular'' achievement goals @c !waitForCommand. Remember, as mentioned above, that we treat events in a run-to-completion manner. However, if the events generated in the plans for @c +!waitForCommand were handled in such a manner, once such an event is handled, it would prevent any other event from being handled, as it keeps on generating @c +!waitForCommand events. For this purpose, we include new focus achievement goals @!!waitForCommand, which generate an event @ +!waitForCommand, but do not require it to be handled immediately. In this sense, the last formula in the body of the plans reads as ''achieve the goal @c waitForCommand, if no other event has to be handled.''

Now, the code for the autopilot is almost complete. There are just two small things missing. First of all, while the events for @c waitForCommand are now handled in the required manner, it would still require the user to ''actively'' activate these plans by issuing the first @c waitForCommand event. A better solution is to give @!waitForCommand as the initial goal of the agent:

 * @snippet autopilot.asl initial goal

Thus, the agent initially waits for instructions. Once the user inputs actual instructions, these get handled (as described above), and, once all instructions are handled, the agent goes back into the ''wait for command'' cycle until further instructions arrive.

Another initial setting we should consider is the value of @c tolerance. This can be easily achieved giving it as an initial belief of the agent:

 * @snippet autopilot.asl initial beliefs

This completes the actual AgentSpeak code. In order to make it translatable, we need to provide some further information in a configuration file (see @c autopilot.conf).

The first two sections of the configuration file are straightforward. Here, the name of the input AgentSpeak file and the names of the output files are given (they can also be provided or overwritten by command line parameters). Besides the actual C code (@c output), a header file for this C code (@c header) and a header file for the basic functions assumed to be provided by the environment (@c basics) is generated.

 * @snippet autopilot.conf settings

Note that setting the @c max_events parameter results in the generated code employing an event queue. This is a convenience feature for testing allowing sequences of events to be fed into the generated code directly, but typically would not be used in actual code (see further examples mentioned below). 

Next, a list of files to be listed as includes in the generated files (if necessary) can be given.

 * @snippet autopilot.conf includes

The next section of the configuration file is used to give a list of the basic functions assumed to be provided by the environment including their types.

 * @snippet autopilot.conf basic actions

These functions need to be implemented ''somewhere else'' already. For an example, in @c actions.c we find:

 * @snippet actions.c closeEnough

The next section of the configuration file is used to give type information for any further elements used (this is necessary as AgentSpeak is untyped whereas C is typed). 

 * @snippet autopilot.conf types

Note also that we define some types in @c types.h, which we include in the generated files as outlined above. These type definitions look as follows

 * @snippet types.h typedefs

The next section of the configuration file declares elements of the belief base that can be directly set by the environment. This also resolves the question from above regarding @c myPosition.

 * @snippet autopilot.conf settables

The @c next_step() function of the generated code calls the function @c updateBeliefs(). This function is implemented by the environment and provides a hook in order to set any elements of the belief base as necessary. In our case, the current position of the UAV is typically determined by some state estimation subsystem, and, say, stored in the variable @c currentPosition. The @c updateBeliefs() function can then be used to transfer this value into the belief base as follows:

 * @snippet actions.c updateBeliefs

Finally, it typically does not make sense for all possible events to be triggerable by the user. We therefore can restrict the triggerable events by setting the following section in the configuration file:

 * @snippet autopilot.conf triggerables

Note that this also includes an event @c +tolerance which has no corresponding plan, but which can be used to change the value of @c tolerance in the belief base.

This concludes the example walk-through. In order to build the example, you can use @c make with the provided Makefile.

If you want to go on from here,
- you can have a look at a real-world version of the autopilot used in the <a href="https://github.com/OxfordUAVAutonomy/tum_ardrone">modified tum_ardrone package</a>, which can be found in <a href="https://github.com/OxfordUAVAutonomy/AgentSpeakTranslator/test/autopilot/">test/autopilot/</a>, 
- you can have look at another simplified example about exploration, which can be found in <a href="https://github.com/OxfordUAVAutonomy/AgentSpeakTranslator/test/exploration/">test/exploration/</a>,
- or, you might be interested in what the translator does with the files described above, which can be found at @subpage overview.

@page overview Code Overview

The translation proceeds in three main stages
-# Parsing of the configuration file and the AgentSpeak code.
  - The parser for the config file is generated by Flex and Bison from @ref config_lexer.l and @ref config_parser.y. The parser is called via @ref config_parser_drivert and the configuration information is stored in @ref agent_configt.
  - The parser for AgentSpeak is generated by Flex and Bison from @ref agentspeak_lexer.l and @ref agentspeak_parser.y. The parser is called via @ref agentspeak_parser_drivert and the resulting agent is stored in @ref agentt.
  - @sa agentspeak_syntax.h atom_buildert agent_buildert
-# Analyzing the AgentSpeak code in order to determine some simple but necessary information relevant for translation.
  - The different analyses performed are stored in @ref agent_analysist, where an overview is given.
-# Translation of the AgentSpeak code to C
  - Here, the main work is delegated to @ref agent_translatort, where a detailled description of the translation is given.
  - Some substeps of the translation are delegated to @ref trigger_translatort and @ref plan_translatort.
  - @sa name_translation.h.
 */
