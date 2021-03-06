/**********************************************************************

   Audacity - A Digital Audio Editor
   Copyright 1999-2009 Audacity Team
   File License: wxWidgets

   Dan Horgan

******************************************************************//**

\file BatchEvalCommand.cpp
\brief Contains definitions for the BatchEvalCommand class

*//*******************************************************************/

#include "../Audacity.h"
#include "BatchEvalCommand.h"
#include "CommandContext.h"

wxString BatchEvalCommandType::BuildName()
{
   return wxT("BatchCommand");
}

void BatchEvalCommandType::BuildSignature(CommandSignature &signature)
{
   auto commandNameValidator = make_movable<DefaultValidator>();
   signature.AddParameter(wxT("CommandName"), wxT(""), std::move(commandNameValidator));
   auto paramValidator = make_movable<DefaultValidator>();
   signature.AddParameter(wxT("ParamString"), wxT(""), std::move(paramValidator));
   auto macroValidator = make_movable<DefaultValidator>();
   signature.AddParameter(wxT("MacroName"), wxT(""), std::move(macroValidator));
}

OldStyleCommandPointer BatchEvalCommandType::Create(std::unique_ptr<CommandOutputTargets> && WXUNUSED(target))
{
   return std::make_shared<BatchEvalCommand>(*this);
}

bool BatchEvalCommand::Apply(const CommandContext & context)
{

   wxString macroName = GetString(wxT("MacroName"));
   if (macroName != wxT(""))
   {
      MacroCommands batch;
      batch.ReadMacro(macroName);
      return batch.ApplyMacro();
   }

   wxString cmdName = GetString(wxT("CommandName"));
   wxString cmdParams = GetString(wxT("ParamString"));

   // Create a Batch that will have just one command in it...
   MacroCommands Batch;
   bool bResult = Batch.ApplyCommand(cmdName, cmdParams, &context);
   // Relay messages, if any.
   wxString Message = Batch.GetMessage();
   if( !Message.IsEmpty() )
      context.Status( Message );
   return bResult;
}

BatchEvalCommand::~BatchEvalCommand()
{ }
