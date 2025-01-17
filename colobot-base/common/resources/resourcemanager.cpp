/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2023, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsitec.ch; http://colobot.info; http://github.com/colobot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://gnu.org/licenses
 */


#include "common/resources/resourcemanager.h"

#include "common/config.h"

#if PLATFORM_WINDOWS
    #include "common/system/system_windows.h"
#endif

#include "common/logger.h"

#include <physfs.h>

#include <regex>


CResourceManager::CResourceManager(const char *argv0)
{
    if (!PHYSFS_init(argv0))
    {
        PHYSFS_ErrorCode errorCode = PHYSFS_getLastErrorCode();
        GetLogger()->Error("Error while initializing physfs: %s\n", PHYSFS_getErrorByCode(errorCode));
        assert(false);
    }
    PHYSFS_permitSymbolicLinks(1);
}


CResourceManager::~CResourceManager()
{
    if (PHYSFS_isInit())
    {
        if (!PHYSFS_deinit())
        {
            PHYSFS_ErrorCode errorCode = PHYSFS_getLastErrorCode();
            GetLogger()->Error("Error while deinitializing physfs: %s\n", PHYSFS_getErrorByCode(errorCode));
        }
    }
}

std::string CResourceManager::CleanPath(const std::filesystem::path& path)
{
    return std::regex_replace(path.generic_u8string(), std::regex("(.*)/\\.\\./"), "");
}


bool CResourceManager::AddLocation(const std::string &location, bool prepend, const std::string &mountPoint)
{
    if (!PHYSFS_mount(location.c_str(), mountPoint.c_str(), prepend ? 0 : 1))
    {
        PHYSFS_ErrorCode errorCode = PHYSFS_getLastErrorCode();
        GetLogger()->Error("Error while mounting \"%s\": %s\n", location.c_str(), PHYSFS_getErrorByCode(errorCode));
        return false;
    }

    return true;
}


bool CResourceManager::RemoveLocation(const std::string &location)
{
    if (!PHYSFS_unmount(location.c_str()))
    {
        PHYSFS_ErrorCode errorCode = PHYSFS_getLastErrorCode();
        GetLogger()->Error("Error while unmounting \"%s\": %s\n", location.c_str(), PHYSFS_getErrorByCode(errorCode));
        return false;
    }

    return true;
}

std::vector<std::string> CResourceManager::GetLocations()
{
    std::vector<std::string> ret;
    char **list = PHYSFS_getSearchPath();
    for (char **it = list; *it != nullptr; ++it)
        ret.push_back(*it);
    PHYSFS_freeList(list);
    return ret;
}

bool CResourceManager::LocationExists(const std::string& location)
{
    auto locations = GetLocations();
    auto it = std::find(locations.cbegin(), locations.cend(), location);
    return it != locations.cend();
}

bool CResourceManager::SetSaveLocation(const std::string &location)
{
    if (!PHYSFS_setWriteDir(location.c_str()))
    {
        PHYSFS_ErrorCode errorCode = PHYSFS_getLastErrorCode();
        GetLogger()->Error("Error while setting save location to \"%s\": %s\n", location.c_str(), PHYSFS_getErrorByCode(errorCode));
        return false;
    }

    return true;
}

std::string CResourceManager::GetSaveLocation()
{
    if (PHYSFS_isInit())
    {
        return PHYSFS_getWriteDir();
    }
    return "";
}

std::unique_ptr<CSDLFileWrapper> CResourceManager::GetSDLFileHandler(const std::string &filename)
{
    return std::make_unique<CSDLFileWrapper>(CleanPath(filename));
}

std::unique_ptr<CSDLMemoryWrapper> CResourceManager::GetSDLMemoryHandler(const std::string &filename)
{
    return std::make_unique<CSDLMemoryWrapper>(CleanPath(filename));
}

std::unique_ptr<CSNDFileWrapper> CResourceManager::GetSNDFileHandler(const std::string &filename)
{
    return std::make_unique<CSNDFileWrapper>(CleanPath(filename));
}


bool CResourceManager::Exists(const std::filesystem::path& filename)
{
    if (PHYSFS_isInit())
    {
        return PHYSFS_exists(CleanPath(filename).c_str());
    }
    return false;
}

bool CResourceManager::DirectoryExists(const std::string& directory)
{
    if (PHYSFS_isInit())
    {
        PHYSFS_Stat statbuf;
        PHYSFS_stat(CleanPath(directory).c_str(), &statbuf);
        return (PHYSFS_exists(CleanPath(directory).c_str()) && (statbuf.filetype == PHYSFS_FILETYPE_DIRECTORY));
    }
    return false;
}

bool CResourceManager::CreateNewDirectory(const std::string& directory)
{
    if (PHYSFS_isInit())
    {
        return PHYSFS_mkdir(CleanPath(directory).c_str());
    }
    return false;
}

bool CResourceManager::RemoveExistingDirectory(const std::string& directory)
{
    if (PHYSFS_isInit())
    {
        std::string path = CleanPath(directory);
        for (auto file : ListFiles(path))
        {
            if (PHYSFS_delete((path + "/" + file).c_str()) == 0)
                return false;
        }
        return PHYSFS_delete(path.c_str()) != 0;
    }
    return false;
}

std::vector<std::string> CResourceManager::ListFiles(const std::string &directory, bool excludeDirs)
{
    std::vector<std::string> result;

    if (PHYSFS_isInit())
    {
        char **files = PHYSFS_enumerateFiles(CleanPath(directory).c_str());

        for (char **i = files; *i != nullptr; i++)
        {
            if (excludeDirs)
            {
                PHYSFS_Stat statbuf;
                std::string path = CleanPath(directory) + "/" + (*i);
                PHYSFS_stat(path.c_str(), &statbuf);
                if (statbuf.filetype == PHYSFS_FILETYPE_DIRECTORY) continue;
            }
            result.push_back(*i);
        }

        PHYSFS_freeList(files);
    }

    return result;
}

std::vector<std::string> CResourceManager::ListDirectories(const std::string &directory)
{
    std::vector<std::string> result;

    if (PHYSFS_isInit())
    {
        char **files = PHYSFS_enumerateFiles(CleanPath(directory).c_str());

        for (char **i = files; *i != nullptr; i++)
        {
            PHYSFS_Stat statbuf;
            std::string path = CleanPath(directory) + "/" + (*i);
            PHYSFS_stat(path.c_str(), &statbuf);
            if (statbuf.filetype == PHYSFS_FILETYPE_DIRECTORY)
            {
                result.push_back(*i);
            }
        }

        PHYSFS_freeList(files);
    }

    return result;
}

long long CResourceManager::GetFileSize(const std::string& filename)
{
    if (PHYSFS_isInit())
    {
        PHYSFS_File* file = PHYSFS_openRead(CleanPath(filename).c_str());
        if(file == nullptr) return -1;
        long long size = PHYSFS_fileLength(file);
        PHYSFS_close(file);
        return size;
    }
    return -1;
}

long long CResourceManager::GetLastModificationTime(const std::string& filename)
{
    if (PHYSFS_isInit())
    {
        PHYSFS_Stat statbuf;
        PHYSFS_stat(CleanPath(filename).c_str(), &statbuf);
        return statbuf.modtime;
    }
    return -1;
}

bool CResourceManager::Remove(const std::string& filename)
{
    if (PHYSFS_isInit())
    {
        return PHYSFS_delete(filename.c_str()) != 0;
    }
    return false;
}
