{
    "targets": [
        {
            "target_name": "sem",
            "sources": [
                "src/sem.cc", 
                "src/sem_acquireworker.cc", 
                "src/sem_timedwait_osx.cc"
                ],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")"
            ]
        }
    ]
}