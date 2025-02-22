/*
 * HCenterPlugin.h - declaration of HCenterPlugin class
 *
 * Copyright (c) 2017-2019 Tobias Junghans <tobydox@veyon.io>
 *
 * This file is part of Veyon - https://veyon.io
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#pragma once

#include "CommandLinePluginInterface.h"
#include "ConfigurationPagePluginInterface.h"
#include "HCenterConfiguration.h"
#include "NetworkObjectDirectoryPluginInterface.h"
#include "UserGroupsBackendInterface.h"

class HCenterDirectory;

class HCenterPlugin : public QObject, PluginInterface,
		CommandLinePluginInterface,
		NetworkObjectDirectoryPluginInterface,
		UserGroupsBackendInterface,
		ConfigurationPagePluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "io.veyon.Veyon.Plugins.HCenter")
	Q_INTERFACES(PluginInterface
				 CommandLinePluginInterface
				 NetworkObjectDirectoryPluginInterface
				 UserGroupsBackendInterface
				 ConfigurationPagePluginInterface)
public:
	HCenterPlugin( QObject* parent = nullptr );
	~HCenterPlugin() override;

	Plugin::Uid uid() const override
	{
		return QStringLiteral("6f0a491e-c1c6-4338-8244-f823b0bf8670");
	}

	QVersionNumber version() const override
	{
		return QVersionNumber( 1, 2 );
	}

	QString name() const override
	{
		return QStringLiteral( "LDAP Basic" );
	}

	QString description() const override
	{
		return tr( "Basic LDAP/AD support for Veyon" );
	}

	QString vendor() const override
	{
		return QStringLiteral( "Veyon Community" );
	}

	QString copyright() const override
	{
		return QStringLiteral( "Tobias Junghans" );
	}

	void upgrade( const QVersionNumber& oldVersion ) override;

	QString commandLineModuleName() const override
	{
		return QStringLiteral( "ldap" );
	}

	QString commandLineModuleHelp() const override
	{
		return tr( "Commands for configuring and testing LDAP/AD integration" );
	}

	QStringList commands() const override;
	QString commandHelp( const QString& command ) const override;

	QString directoryName() const override
	{
		return tr( "%1 (load computers and locations from LDAP/AD)" ).arg( name() );
	}

	NetworkObjectDirectory* createNetworkObjectDirectory( QObject* parent ) override;

	QString userGroupsBackendName() const override
	{
		return tr( "%1 (load users and groups from LDAP/AD)" ).arg( name() );
	}

	void reloadConfiguration() override;

	QStringList userGroups( bool queryDomainGroups ) override;
	QStringList groupsOfUser( const QString& username, bool queryDomainGroups ) override;

	ConfigurationPage* createConfigurationPage() override;


public slots:
	CommandLinePluginInterface::RunResult handle_autoconfigurebasedn( const QStringList& arguments );
	CommandLinePluginInterface::RunResult handle_query( const QStringList& arguments );
	CommandLinePluginInterface::RunResult handle_help( const QStringList& arguments );

private:
	enum {
		MaximumPlaintextPasswordLength = 64
	};

	HCenterClient& ldapClient();
	HCenterDirectory& ldapDirectory();

	HCenterConfiguration m_configuration;
	HCenterClient* m_ldapClient;
	HCenterDirectory* m_ldapDirectory;
	QMap<QString, QString> m_commands;

};
